#include "interpreter.h"

Interpreter::Interpreter(QObject* Parent) : QObject{Parent}
{
	Py_Initialize();
}

Interpreter::~Interpreter()
{
	moduleDict = object();
	moduleMain = object();
}

void Interpreter::Shutdown()
{
	modDictInst()["abort"]();
	//modDictInst()["_thread.interrupt_main(signum=signal.SIGKILL)"];
	//modMainInst()["_thread.interrupt_main(signum=signal.SIGKILL)"];
	emit trainFinished();
}

void Interpreter::PreInit(QVariantList RolesList, QString ModelPath, QString SavePath, int Batches, int Epochs)
{
	this->RolesList = RolesList;
	this->ModelPath = ModelPath;
	this->SavePath = SavePath;
	this->Batches = Batches;
	this->Epochs = Epochs;
}

void Interpreter::Init()
{
	QFile file("train.py");
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "\nOpen file error " << file.errorString();
		return;
	}
	else
	{
		QByteArray script = file.readAll();
		file.close();
		object ignore = exec(script.constData(), modDictInst(), modDictInst());
		IsInited = true;
	}
}

void Interpreter::doTrain()
{
	if(!IsInited)
	{
		Init();
	}
	list PyRolesList;
	for(const auto &RoleId : RolesList)
	{
		PyRolesList.append(RoleId.toInt());
	}
	modDictInst()["train_process"](PyRolesList, ModelPath.toUtf8().toStdString().c_str(),
				       ModelPath.toUtf8().toStdString().c_str(), SavePath.toUtf8().toStdString().c_str(),
				       Batches, Epochs);
	emit trainFinished();
}

object& Interpreter::modMainInst()
{
	if( moduleMain.is_none() )
		moduleMain = import( "__main__" );
	return moduleMain;
}

object& Interpreter::modDictInst()
{
	if(moduleDict.is_none())
		moduleDict = modMainInst().attr("__dict__");
	return moduleDict;
}

