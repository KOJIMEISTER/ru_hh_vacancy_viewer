import re
from transformers import pipeline

def CleanText(text):
    text = text.lower()
    text = re.sub(r"</?\w{1,}>", "", text)
    text = re.sub(r"http\S+", "",text)
    text = re.sub(r"[^a-zA-Z\u0400-\u04ff+#]+", " ", text)
    html = re.compile(r'<.*?>')
    text = html.sub(r'',text)
    punctuations = '@!?&*[]-%.:/();$=><|{}^' + "'`" + '_'
    for p in punctuations:
        text = text.replace(p,'')
    emoji_pattern = re.compile("["
                           u"\U0001F600-\U0001F64F"  # emoticons
                           u"\U0001F300-\U0001F5FF"  # symbols & pictographs
                           u"\U0001F680-\U0001F6FF"  # transport & map symbols
                           u"\U0001F1E0-\U0001F1FF"  # flags (iOS)
                           u"\U00002702-\U000027B0"
                           u"\U000024C2-\U0001F251"
                           "]+", flags=re.UNICODE)
    text = emoji_pattern.sub(r'', text) #Removing emojis
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

classifier = pipeline(task="text-classification", model="model/")

def Classify(text):
    text = CleanText(text)
    return classifier(text, batch_size=1, truncation="only_first")