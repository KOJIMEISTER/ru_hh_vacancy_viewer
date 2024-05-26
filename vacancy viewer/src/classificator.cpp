#include "classificator.h"

Classificator::Classificator(QObject* Parent) : QObject{Parent}
{
	LoadConfig();
	Py_Initialize();
	QFile file(":/classify");
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "\nOpen file error " << file.errorString();
		return;
	}
	QByteArray script = file.readAll();
	file.close();
	object ignore = exec(script.constData(), modDictInst(), modDictInst());
}

Classificator::~Classificator()
{
	Stop();
	moduleDict = object();
	moduleMain = object();
}

QPair<QString, int> Classificator::doClassification(QString Text)
{
	list ListPred = extract<list>(modDictInst()["Classify"](Text.toUtf8().toStdString().c_str()));
	dict DictPred = extract<dict>(ListPred[0]);
	QPair<QString, int> Prediction(extract<const char*>(DictPred.get("label")), static_cast<int>(extract<float>(DictPred.get("score"))*100));
	return Prediction;
}

object& Classificator::modMainInst()
{
	if( moduleMain.is_none() )
		moduleMain = import( "__main__" );
	return moduleMain;
}

object& Classificator::modDictInst()
{
	if(moduleDict.is_none())
		moduleDict = modMainInst().attr("__dict__");
	return moduleDict;
}

void Classificator::LoadConfig()
{
	QFile Config("classificator_config.json");
	if(Config.open(QFile::ReadOnly))
	{
		QJsonDocument Json = QJsonDocument::fromJson(Config.readAll());
		Config.close();
		for(const auto &Id : Json["available_ids"].toArray())
		{
			AllowedRoleIdList.push_back(Id.toInt());
		}
		MinAccuracy = Json["min_accuracy"].toInt();
	}
}

void Classificator::Stop()
{
	StoppingState = true;
	SemToPredict.release(1);
}

void Classificator::RecieveProcess()
{
	while(true)
	{
		SemToPredict.acquire(1);
		if(StoppingState)
		{
			return;
		}
		auto ToPredict = GetToPredict();
		auto Predict = doClassification(ToPredict.second);
		if(Predict.second > MinAccuracy)
		{
			emit sendPredict(ToPredict.first,  Predict.first);
		}
	}
}

bool Classificator::IsRoleIdAllow(int RoleId)
{
	for(const auto& Id : AllowedRoleIdList)
	{
		if(Id == RoleId)
		{
			return true;
		}
	}
	return false;
}

void Classificator::RequestPredict(int Index, QString Text, int RoleId)
{
	if(IsRoleIdAllow(RoleId))
	{
		PutToPredict(Index, Text);
	}
}

void Classificator::PutToPredict(int Index, QString Text)
{
	MutexToPredict.lock();
	StackToPredict.push({Index, Text});
	SemToPredict.release(1);
	MutexToPredict.unlock();
}

QPair<int, QString> Classificator::GetToPredict()
{
	QPair<int, QString> ToPredict{0, ""};
	MutexToPredict.lock();
	ToPredict = StackToPredict.pop();
	MutexToPredict.unlock();
	return ToPredict;
}

