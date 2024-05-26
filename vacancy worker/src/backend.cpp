#include "backend.h"

Backend::Backend(QObject *parent)
	: QObject{parent}
{
	Net = new RemoteDatabase("RemoteDBBackend", this);
	CurIdx = -1;
	connect(&LoaderThread, &QThread::finished, this, &Backend::toIdleAfterThreadDone);
}

Backend::~Backend()
{
	if(Net != nullptr)
	{
		delete Net;
	}
	if(Loader != nullptr)
	{
		Loader->Stop();
		LoaderThread.quit();
		LoaderThread.wait();
		delete Loader;
	}
	if(Trainer != nullptr)
	{
		Trainer->Shutdown();
		TrainerThread.quit();
		TrainerThread.exit();
		TrainerThread.wait();
		delete Trainer;
	}
}

void Backend::trainFinished()
{
	IsTrainExecute = false;
	emit isTrainExecuteChanged();
}

void Backend::startTrain(QVariantList RolesList, QString ModelPath, QString SavePath, int Batches, int Epochs)
{
	if(!TrainerThread.isRunning())
	{
		if(Trainer != nullptr)
		{
			delete Trainer;
		}
		IsTrainExecute = true;
		emit isTrainExecuteChanged();
		Trainer = new Interpreter();
		Trainer->PreInit(RolesList, ModelPath, SavePath, Batches, Epochs);
		connect(&TrainerThread, &QThread::started, Trainer, &Interpreter::doTrain);
		connect(Trainer, &Interpreter::trainFinished, this, &Backend::trainFinished);
		connect(Trainer, &Interpreter::trainFinished, &TrainerThread, &QThread::quit);
		Trainer->moveToThread(&TrainerThread);
		TrainerThread.start();
	}
}

void Backend::toIdleAfterThreadDone()
{
	CurState = "Idle";
	emit curStateChanged();
}

void Backend::getChangedState(QString Stage)
{
	CurState = Stage;
	emit curStateChanged();
	if(Stage == "Stop")
	{
		LoaderThread.quit();
	}
}

void Backend::getChangedValue(int Value, int MaxValue)
{
	CurValue = Value;
	emit curValueChanged();
	this->MaxValue = MaxValue;
	emit maxValueChanged();
}

void Backend::getChangedValueHandle(int ValueHandle, int MaxValueHandle)
{
	CurValueHandle = ValueHandle;
	emit curValueHandleChanged();
	this->MaxValueHandle = MaxValueHandle;
	emit maxValueHandleChanged();
}

void Backend::stopRefresh()
{
	Loader->Stop();
	LoaderThread.quit();
}

void Backend::refreshVacs(QVariantList RolesList, int Period)
{
	if(!LoaderThread.isRunning())
	{
		if(Loader != nullptr)
		{
			delete Loader;
		}
		Loader = new LoaderProg(RolesList, Period);
		connect(&LoaderThread, &QThread::started, Loader, &LoaderProg::Start);
		connect(Loader, &LoaderProg::StageChanged, this, &Backend::getChangedState);
		connect(Loader, &LoaderProg::ValueChanged, this, &Backend::getChangedValue);
		connect(Loader, &LoaderProg::ValueChangedHandle, this, &Backend::getChangedValueHandle);
		Loader->moveToThread(&LoaderThread);
		LoaderThread.start();
	}
}

QString Backend::getVacCount()
{
	QString Number = "0";
	Number = Net->GetVacCount();
	return Number;
}

void Backend::clearHistory()
{
	HistoryVac.clear();
	CurIdx = -1;
}

void Backend::deleteRoleDir(QString DirName)
{
	Net->DeleteRoleDir(DirName);
}

QVariantList Backend::getAllRoleDirs()
{
	QVariantList DirsList;
	QSqlQuery Query = Net->GetAllRoleDirs();
	while(Query.next())
	{
		QVariantMap DirMap;
		DirMap.insert("dirName", Query.value("type_name").toString());
		DirMap.insert("count", Query.value("count").toString());
		DirsList.append(DirMap);
	}
	Query.clear();
	return DirsList;
}

QVariantList Backend::getDatasetRoles()
{
	QVariantList Roles;
	QSqlQuery Query = Net->GetDatasetRoles();
	while(Query.next())
	{
		QVariantMap RoleMap;
		RoleMap.insert("roleId", Query.value("role_id"));
		RoleMap.insert("roleName", Query.value("role_name"));
		RoleMap.insert("roleCount", Query.value("count"));
		Roles.append(RoleMap);
	}
	Query.clear();
	return Roles;
}

void Backend::deleteDatasetRole(int RoleId)
{
	Net->DeleteDatasetRole(RoleId);
}

void Backend::clearVacLabel(int VacId)
{
	Net->ClearVacLabel(VacId);
	auto Map = HistoryVac.at(CurIdx);
	Map.insert("vacMarked", false);
	HistoryVac.replace(CurIdx, Map);
}

void Backend::previousVac()
{
	if(CurIdx > 0)
	{
		--CurIdx;
	}
}

void Backend::nextVac(int RoleId)
{
	if(CurIdx != HistoryVac.size()-1)
	{
		++CurIdx;
	}
	else
	{
		QVariantMap VacData;
		QSqlQuery Query = Net->GetVacancyToClassify(RoleId);
		int VacId = 0;
		if(Query.next())
		{
			VacId = Query.value("vac_id").toInt();
			VacData.insert("vacId", Query.value("vac_id").toString());
			VacData.insert("vacDesc", Query.value("vac_description").toString());
			VacData.insert("vacName", Query.value("vac_name").toString());
			VacData.insert("vacMarked", false);
		}
		Query.clear();
		Query = Net->GetVacancyTechs(VacId);
		QVariantList VacTechs;
		while(Query.next())
		{
			VacTechs.push_back(Query.value("tech_name").toString());
		}
		Query.clear();
		VacData.insert("vacTechs", VacTechs);
		bool IsExists = false;
		for(const auto& Vac : HistoryVac)
		{
			if(Vac.value("vacId") == VacData.value("vacId"))
			{
				IsExists = true;
				break;
			}
		}
		if(HistoryVac.size() == 0 or !IsExists)
		{
			HistoryVac.push_back(VacData);
			++CurIdx;
		}
	}
}

QVariantMap Backend::getCurrentVac()
{
	if(HistoryVac.size() == 0)
	{
		return QVariantMap();
	}
	else
	{
		return HistoryVac.at(CurIdx);
	}
}

void Backend::addVacToDataset(int VacId, QString Name)
{
	Net->AddVacToDataset(VacId, Name);
	auto Map = HistoryVac.at(CurIdx);
	Map.insert("vacMarked", true);
	HistoryVac.replace(CurIdx, Map);
}

void Backend::setVacSkipped(int VacId)
{
	Net->SetVacSkipped(VacId);
}

QVariantList Backend::getDirs(int RoleId)
{
	QVariantList DirsList;
	QSqlQuery Query = Net->GetDirs(RoleId);
	while(Query.next())
	{
		DirsList.append(Query.value("type_name"));
	}
	Query.clear();
	return DirsList;
}

bool Backend::addDir(QString Name, int RoleId)
{
	return Net->AddDir(Name, RoleId);
}

QVariantList Backend::getAllRoles()
{
	QVariantList RolesList;
	QSqlQuery Roles = Net->GetAllRoles();
	while(Roles.next())
	{
		QVariantMap SpecMap;
		SpecMap.insert("name", Roles.value("role_name"));
		SpecMap.insert("roleId", Roles.value("vac_role_id"));
		SpecMap.insert("count", Roles.value("count"));
		SpecMap.insert("checked", false);
		RolesList.push_back(SpecMap);
	}
	Roles.clear();
	return RolesList;
}

QString Backend::getVacancyDescription(int VacId)
{
	QString Description = Net->GetVacancyDescrition(VacId);
	return Description;
}

QString Backend::reformateText(QString TextToFormate)
{
	QString FormatedText;
	QRegularExpression Requirments("(<\\w+>)*[а-я\\w\\- ,.«»:]*(требования|(наши пожелания)|обладают|обязанности|((вам|тебе|чем|что|которые) предстоит)|ожидаем|ждем|требуется|(наши ожидания)|(нам важно)|(ты будешь)|(вы будете)|(нужно (будет)? делать)|(задачи (ждут вас)|(вас ждут))|(зона ответственности)|(необходимые компетенции))[а-я\\w\\- ,.«»:]*", QRegularExpression::PatternOption::CaseInsensitiveOption);
	QRegularExpression Conditions("(<\\w+>)*[а-я\\w\\- ,.«»:]*(предлагаем|предложить|предлагает|условия)[а-я\\w\\- ,.«»:]*", QRegularExpression::PatternOption::CaseInsensitiveOption);
	QRegularExpressionMatch ReqMatch = Requirments.match(TextToFormate);
	QRegularExpressionMatch CondMatch = Conditions.match(TextToFormate);
	qsizetype ReqPos = -1, CondPos = -1;
	if(ReqMatch.hasMatch())
	{
		ReqPos = ReqMatch.capturedStart();
	}
	if(CondMatch.hasMatch())
	{
		CondPos = CondMatch.capturedStart();
	}
	if(ReqPos != -1)
	{
		if(ReqPos <= CondPos)
		{
			QString Important = TextToFormate.mid(ReqPos, CondPos-ReqPos);
			TextToFormate = TextToFormate.remove(ReqPos, CondPos-ReqPos);
			FormatedText.append(Important);
			FormatedText.append(TextToFormate);
			return FormatedText;
		}
		else
		{
			QString Important = TextToFormate.mid(ReqPos, TextToFormate.length()-ReqPos);
			TextToFormate = TextToFormate.remove(ReqPos, TextToFormate.length()-ReqPos);
			FormatedText.append(Important);
			FormatedText.append(TextToFormate);
			return FormatedText;
		}
	}
	else
	{
		return TextToFormate;
	}
}
