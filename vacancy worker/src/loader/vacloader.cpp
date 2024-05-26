#include "vacloader.h"

VacLoader::VacLoader(QVariantList& RolesList, int Period, QObject* Parent) : QObject{Parent}
{
	this->Period = Period;
	VacLoadArgs = GetExtra(RolesList);
	MaxValue = VacLoadArgs.size();
	AmmProviders = 2;
	AmmHandlers = 10;
	ActiveThreads = new bool[AmmProviders + AmmHandlers] {false};
	Providers = new std::thread * [AmmProviders];
	Handlers = new std::thread * [AmmHandlers];
}

VacLoader::~VacLoader()
{
	if (Inited)
	{
		for (int i = 0; i < AmmProviders; ++i)
		{
			Providers[i]->detach();
			delete Providers[i];
			Providers[i] = nullptr;
		}
		for (int i = 0; i < AmmHandlers; ++i)
		{
			Handlers[i]->detach();
			delete Handlers[i];
			Handlers[i] = nullptr;
		}
	}
	delete[] Providers;
	delete[] Handlers;
	delete[] ActiveThreads;
}

void VacLoader::Stop()
{
	Active = false;
	VacIdsSem.release(9999);
}

void VacLoader::Load()
{
	qDebug() << "\nStart loading\n";
	if (Net == nullptr or Query == nullptr)
	{
		return;
	}
	for (int i = 0; i < AmmProviders; ++i)
	{
		ActiveThreads[i] = true;
		Providers[i] = new std::thread(&VacLoader::ProviderWork, this, i);
	}
	for (int i = 0, j = AmmProviders; i < AmmHandlers; ++i, ++j)
	{
		Handlers[i] = new std::thread(&VacLoader::HandlerWork, this, j);
	}
	Inited = true;
	while (!IsEnd() and Active)
	{
		emit ChangedVal(MaxValue - VacLoadArgs.size(), MaxValue);
		emit ChangedValHandle(MaxValueHandlers - VacParseInfo.size(), MaxValueHandlers);
		Sleep(1000);
	}
}

bool VacLoader::IsEnd()
{
	for (int i = 0; i < AmmProviders + AmmProviders; ++i)
	{
		if (ActiveThreads[i])
		{
			return false;
		}
	}
	if (!VacParseInfo.empty())
	{
		return false;
	}
	return true;
}

void VacLoader::JoinNetwork(Network* Net)
{
	this->Net = Net;
}

void VacLoader::JoinQueryHandler(QueryHandler* Query)
{
	this->Query = Query;
}

ExtraInfo VacLoader::GetVacLoadArgsItem()
{
	ExtraInfo Args;
	VacLoadArgsMtx.lock();
	if (!VacLoadArgs.empty())
	{
		Args = VacLoadArgs.front();
		VacLoadArgs.pop();
	}
	VacLoadArgsMtx.unlock();
	return Args;
}

void VacLoader::ProviderWork(const int& ProviderId)
{
	while(Active)
	{
		ExtraInfo Args = GetVacLoadArgsItem();
		if (Args.IsEmpty())
		{
			break;
		}
		std::string Url = SearchUrl;
		Url += "&professional_roles=";
		Url += Args.Role;
		Url += "&period=";
		Url += std::to_string(Period);
		Url += "&schedule=";
		Url += Args.Schedule;
		Url += "&experience=";
		Url += Args.Experience;
		Url += "&employment=";
		Url += Args.Employment;
		int CurPage = 0;
		int MaxPage = 1;
		while(Active)
		{
			std::string PagedUrl = Url;
			PagedUrl += "&page=";
			PagedUrl += std::to_string(CurPage);
			std::string JSONData = "";
			Net->Get(PagedUrl, JSONData);
			if (!JSONData.empty())
			{
				QJsonDocument JSONParsed =QJsonDocument::fromJson(JSONData.c_str());
				if (!JSONParsed["errors"].isUndefined())
				{
					Sleep(1000);
					continue;
				}
				if (MaxPage == 1)
				{
					MaxPage = JSONParsed["pages"].toInt();
				}
				ParseVacIdsFromJson(JSONParsed, Args);
				++CurPage;
			}
			if(CurPage == MaxPage)
			{
				break;
			}
		}
	}
	ActiveThreads[ProviderId] = false;
	qDebug() << "\nProvider #" << ProviderId << " finished";
}

void VacLoader::HandlerWork(const int& HandlerId)
{
	while(Active)
	{
		ActiveThreads[HandlerId] = false;
		ParseFromSearch VacInfo = GetVacId();
		if(!Active)
		{
			break;
		}
		ActiveThreads[HandlerId] = true;
		std::string StrId = std::to_string(VacInfo.Id);
		std::string Url = VacUrl;
		Url += StrId;
		std::string Data;
		Net->Get(Url, Data);
		if (Data.empty())
		{
			PutVacId(VacInfo);
			continue;
		}
		QJsonDocument JSONParsed = QJsonDocument::fromJson(Data.c_str());
		if (!JSONParsed["errors"].isUndefined())
		{
			continue;
		}
		Vacancy Vac;
		Vac = ParseVacancy(JSONParsed);
		Vac.VacancyId = StrId;
		if (!Vac.VacancyName.empty())
		{
			Query->AddVacancyToDB(Vac);
		}
	}
	ActiveThreads[HandlerId] = false;
	qDebug() << "\nHandler #" << HandlerId << " finished";
}

ParseFromSearch VacLoader::GetVacId()
{
	VacIdsSem.acquire();
	ParseFromSearch Info;
	VacIdsMtx.lock();
	if(!VacParseInfo.empty())
	{
		Info = VacParseInfo.front();
		VacParseInfo.pop();
	}
	VacIdsMtx.unlock();
	return Info;
}

void VacLoader::PutVacId(ParseFromSearch Info)
{
	VacIdsMtx.lock();
	VacParseInfo.push(Info);
	++MaxValueHandlers;
	VacIdsMtx.unlock();
	VacIdsSem.release();
}

bool VacLoader::ParseVacIdsFromJson(const QJsonDocument& JSONParsed, ExtraInfo Args)
{
	if (!JSONParsed["items"].isUndefined() and JSONParsed["items"].isArray())
	{
		auto JsonArray = JSONParsed["items"].toArray();
		if(!JsonArray.isEmpty())
		{
			for (const auto Item : JsonArray)
			{
				std::string StrId = Item.toObject().value("id").toString().toStdString();
				int Id = std::stoi(StrId);
				if (!Query->IdsExists(Id))
				{
					PutVacId(ParseFromSearch(Id));
					Query->PutIdsTable(Id);
				}
			}
			return true;
		}
	}
	return false;
}

Vacancy VacLoader::ParseVacancy(const QJsonDocument& JSONParsed)
{
	Vacancy Vac;
	Vac.Area = JSONParsed["area"]["name"].toString().toStdString();
	Vac.Published = JSONParsed["published_at"].toString().toStdString();
	if (!JSONParsed["salary"].isUndefined())
	{
		Vac.PaymentCur = JSONParsed["salary"]["currency"].toString().toStdString();
		if (!JSONParsed["salary"]["from"].isUndefined())
		{
			int Pay = JSONParsed["salary"]["from"].toInt();
			Vac.PaymentFrom = std::to_string(Pay);
		}
		if (!JSONParsed["salary"]["to"].isUndefined())
		{
			int Pay = JSONParsed["salary"]["to"].toInt();
			Vac.PaymentTo = std::to_string(Pay);
		}
		if (!JSONParsed["salary"]["gross"].isUndefined() and JSONParsed["salary"]["gross"].toBool())
		{
			Vac.PaymentGross = true;
		}
	}
	if (!JSONParsed["professional_roles"].toArray().isEmpty())
	{
		Vac.Role = JSONParsed["professional_roles"].toArray().first().toObject().value("id").toString().toStdString();
	}
	Vac.VacancyName = JSONParsed["name"].toString().toStdString();
	Vac.CompanyName = JSONParsed["employer"]["name"].toString().toStdString();
	Vac.Employment = JSONParsed["employment"]["id"].toString().toStdString();
	Vac.Description = JSONParsed["description"].toString().toStdString();
	Vac.Schedule = JSONParsed["schedule"]["id"].toString().toStdString();
	Vac.WorkExp = JSONParsed["experience"]["id"].toString().toStdString();
	for (const auto value : JSONParsed["key_skills"].toArray()) {
		Vac.KeySkills.push_back(value.toObject().value("name").toString().toStdString());
	}
	return Vac;
}
