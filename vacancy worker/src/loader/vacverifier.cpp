#include "vacverifier.h"

VacVerifier::VacVerifier(QVariantList& RolesList, QObject* Parent) : QObject{Parent}
{
	this->RolesList = RolesList;
	AmmHandlers = 12;
	ActiveThreads = new bool[AmmHandlers] {false};
	Handlers = new std::thread * [AmmHandlers];
}

VacVerifier::~VacVerifier()
{
	if (Inited)
	{
		for (int i = 0; i < AmmHandlers; ++i)
		{
			Handlers[i]->detach();
			delete Handlers[i];
			Handlers[i] = nullptr;
		}
	}
	delete[] Handlers;
	delete[] ActiveThreads;
}

void VacVerifier::Stop()
{
	Active = false;
	VacIdsSem.release(9999);
}

void VacVerifier::InitIds()
{
	std::string QueryStr = "select vac_id from vacancy where vac_archived='false' and vac_original='true' and vac_role_id in (";
	int CurRole = 0;
	for(const auto& RoleId : RolesList)
	{
		QueryStr += "'";
		QueryStr += RoleId.toString().toStdString();
		QueryStr += "'";
		++CurRole;
		if(CurRole != RolesList.count())
		{
			QueryStr += ",";
		}
	}
	QueryStr += ")";
	auto IdsList = Query->DoSelect(QueryStr);
	for (const auto& Id : IdsList)
	{
		VacIds.push(std::stoi(Id.at("vac_id")));
	}
	MaxValue = VacIds.size();
	VacIdsSem.release(VacIds.size());
}

void VacVerifier::Verify()
{
	qDebug() << "\nStart verifying\n";
	if (Net == nullptr or Query == nullptr)
	{
		return;
	}
	for (int i = 0; i < AmmHandlers; ++i)
	{
		Handlers[i] = new std::thread(&VacVerifier::HandlerWork, this, i);
	}
	Inited = true;
	while (!IsEnd() and Active)
	{
		emit ChangedVal(MaxValue - VacIds.size(), MaxValue);
		Sleep(1000);
	}
}

bool VacVerifier::IsEnd()
{
	for (int i = 0; i < AmmHandlers; ++i)
	{
		if (ActiveThreads[i])
		{
			return false;
		}
	}
	if (!VacIds.empty())
	{
		return false;
	}
	return true;
}

void VacVerifier::JoinNetwork(Network* Net)
{
	this->Net = Net;
}

void VacVerifier::JoinQueryHandler(QueryHandler* Query)
{
	this->Query = Query;
	InitIds();
}

void VacVerifier::HandlerWork(const int& HandlerId)
{
	while(Active)
	{
		ActiveThreads[HandlerId] = false;
		unsigned int VacId = GetVacId();
		if(!Active)
		{
			break;
		}
		ActiveThreads[HandlerId] = true;
		std::string StrId = std::to_string(VacId);
		std::string Url = VacUrl;
		Url += StrId;
		std::string Data;
		Net->Get(Url, Data);
		if (Data.empty())
		{
			PutVacId(VacId);
			continue;
		}
		QJsonDocument JSONParsed = QJsonDocument::fromJson(Data.c_str());
		if (!JSONParsed["errors"].isUndefined())
		{
			continue;
		}
		Vacancy Vac;
		Vac.Archive = JSONParsed["archived"].toBool();
		Vac.VacancyId = StrId;
		if (Vac.Archive)
		{
			Query->UpdateArchive(Vac);
		}
	}
	ActiveThreads[HandlerId] = false;
	qDebug() << "\nHandler #" << HandlerId << " finished";
}

unsigned int VacVerifier::GetVacId()
{
	VacIdsSem.acquire();
	unsigned int VacId = 0;
	VacIdsMtx.lock();
	if(!VacIds.empty())
	{
		VacId = VacIds.front();
		VacIds.pop();
	}
	VacIdsMtx.unlock();
	return VacId;
}

void VacVerifier::PutVacId(const int& Id)
{
	VacIdsMtx.lock();
	VacIds.push(Id);
	VacIdsMtx.unlock();
	VacIdsSem.release();
}
