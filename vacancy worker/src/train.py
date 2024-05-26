import numpy as np
import pandas as pd
import nltk
from nltk.corpus import stopwords
import re
from transformers import AutoModelForSequenceClassification, AutoTokenizer, TrainingArguments, Trainer, DataCollatorWithPadding
from datasets import Dataset, DatasetDict
import evaluate
import torch
import psycopg2

# Параметры соединения с СУБД

hostname = "localhost"
port = 5432
user = "postgres"
password = "546258"
database = "itddb"

# ---------------------------

nltk.download('stopwords')
sw = stopwords.words('russian')
accuracy = evaluate.load("accuracy")

def CleanText(text):
    text = text.lower()
    html = re.compile(r'<.*?>')
    text = html.sub(r'',text)
    text = re.sub(r"http\S+", "",text)
    text = re.sub(r"[^a-zA-Z\u0400-\u04ff+#]+", " ", text)
    punctuations = '@!?&*[]-%:/();$=><|{}^' + "'`" + '_'
    for p in punctuations:
        text = text.replace(p,'')
    text = [word.lower() for word in text.split() if word.lower() not in sw]
    text = " ".join(text)
    emoji_pattern = re.compile("["
                           u"\U0001F600-\U0001F64F"  # emoticons
                           u"\U0001F300-\U0001F5FF"  # symbols & pictographs
                           u"\U0001F680-\U0001F6FF"  # transport & map symbols
                           u"\U0001F1E0-\U0001F1FF"  # flags (iOS)
                           u"\U00002702-\U000027B0"
                           u"\U000024C2-\U0001F251"
                           "]+", flags=re.UNICODE)
    match = re.search(r"(предлагаем|предложить|предлагает|условия)", text)
    if(match):
        cond = match.span()[0]
    else:
        cond = len(text)
    match = re.search(r"(требования|(наши пожелания)|обладают|обязанности|((вам|тебе|чем|что|которые) предстоит)|ожидаем|ждем|требуется|(наши ожидания)|(нам важно)|(ты будешь)|(вы будете)|(нужно (будет)? делать)|(задачи (ждут вас)|(вас ждут))|(зона ответственности)|(необходимые компетенции))", text)
    if(match):
        req = match.span()[0]
    else:
        req = 0
    if(cond <= req):
        cond = len(text)
    text = text[req:cond]
    return text

def label2tens(val, size):
    tens = np.zeros(size)
    tens[int(val)] = 1
    return tens
    
def compute_metrics(eval_pred):
    predictions, labels = eval_pred
    predictions = np.argmax(predictions, axis=1)
    labels = np.argmax(labels, axis=1)
    return accuracy.compute(predictions=predictions, references=labels)

def load_dataset(tokenizer, roles_list):
    conn = psycopg2.connect(database=database, user=user, password=password, host=hostname, port=port)
    cur = conn.cursor()
    strIds = "("
    curIdx = 0
    for id in roles_list:
        strIds += "'{0}'".format(id)
        curIdx += 1
        if(curIdx != len(roles_list)):
            strIds += ","
    strIds += ")"
    cur.execute("select vac_description as text, type_name as label from vacancy_dataset join vacancy using (vac_id) where vac_role_id in " + strIds)
    rows = cur.fetchall()
    df = pd.DataFrame(columns=["text", "label"])
    for row in rows:
        df.loc[len(df.index)] = {"text":row[0], "label":row[1]}
    labels = df['label'].unique()
    lb2id = {}
    id2lb = {}
    for i in range(0,len(labels)):
        lb2id.update({labels[i]:i})
        id2lb.update({i:labels[i]})
    df['label'] = df['label'].map(lambda x: lb2id[x])
    df['text'] = df['text'].map(CleanText)
    train_df = pd.DataFrame()
    test_df = pd.DataFrame()
    for idx in range(0, len(labels)):
        tosplit = df[df['label']==idx]
        values = []
        for val in tosplit['label'].values:
            values.append(label2tens(val, len(labels)))
        tosplit = tosplit.drop('label', axis=1)
        tosplit = tosplit.sample(frac=1).reset_index(drop=True)
        tosplit['label'] = values
        train_df = pd.concat([train_df, tosplit[:int(len(tosplit)*0.8)]], ignore_index=True)
        test_df = pd.concat([test_df, tosplit[int(len(tosplit)*0.8):]], ignore_index=True)
    train_dataset = Dataset.from_dict({'text':train_df['text'].values, 'label':train_df['label'].values})
    test_dataset = Dataset.from_dict({'text':test_df['text'].values, 'label':test_df['label'].values})
    dataset = DatasetDict({'train':train_dataset, 'test':test_dataset})
    dataset = dataset.map(lambda x: tokenizer(x["text"], truncation=True))
    return dataset, lb2id, id2lb

def load_tokenizer(path):
    if(path != ""):
        tokenizer = AutoTokenizer.from_pretrained(path, model_max_length=512, truncation=True, max_length=512)
    else:
        tokenizer = AutoTokenizer.from_pretrained("apanc/russian-sensitive-topics", model_max_length=512, truncation=True, max_length=512)
    return tokenizer

def load_model(device, lb2id, id2lb, path):
    if(path != ""):
        model = AutoModelForSequenceClassification.from_pretrained(path, num_labels=len(lb2id), ignore_mismatched_sizes=True, id2label=id2lb, label2id=lb2id)
    else:
        model = AutoModelForSequenceClassification.from_pretrained("apanc/russian-sensitive-topics", num_labels=len(lb2id), ignore_mismatched_sizes=True, id2label=id2lb, label2id=lb2id)
    model = model.to(device)
    return model
    
def train_process(roles_list, tokenizer_path="", model_path="", save_path="trained/", batch=8, epochs=5, output_dir="tmp/"):
    print("Train process execute: roles {0}, model/tokenizer path {1}, save path {2}, batch size = {3}, epochs = {4}, temporary dir = {5}".format(roles_list, model_path, save_path, batch, epochs, output_dir))
    print("Load tokenizer")
    tokenizer = load_tokenizer(tokenizer_path)
    print("Load dataset")
    dataset, lb2id, id2lb = load_dataset(tokenizer, roles_list)
    print("Load model")
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    model = load_model(device, lb2id, id2lb, model_path)
    print("Set collator")
    data_collator = DataCollatorWithPadding(tokenizer=tokenizer)
    print("Set train and arguments")
    training_args = TrainingArguments(output_dir=output_dir, learning_rate=2e-5, per_device_train_batch_size=batch, per_device_eval_batch_size=batch, num_train_epochs=epochs, evaluation_strategy="epoch", weight_decay=0.01)
    trainer = Trainer(model=model, args=training_args, train_dataset=dataset["train"], eval_dataset=dataset["test"], tokenizer=tokenizer, data_collator=data_collator,compute_metrics=compute_metrics)
    print("Train started")
    trainer.train()
    print("Save model to {0}".format(save_path))
    model.save_pretrained(save_path)
    print("Save tokenizer to {0}".format(save_path))
    tokenizer.save_pretrained(save_path)
    print("Train process done")