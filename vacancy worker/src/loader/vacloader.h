#pragma once
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QObject>
#include <thread>
#include <queue>
#include <semaphore>
#include <mutex>
#include <iostream>
#include "urlparams.h"
#include "vacancy.hpp"
#include "network.h"
#include "queryhandler.h"

struct ParseFromSearch
{
	ParseFromSearch() {}
	ParseFromSearch(int Id) :
		Id(Id) {}
	int Id;
};

class VacLoader : public QObject
{
	Q_OBJECT
	Network* Net = nullptr;
	QueryHandler* Query = nullptr;
	unsigned int AmmProviders = 1;
	unsigned int AmmHandlers = 1;
	bool* ActiveThreads = nullptr;
	std::thread** Providers = nullptr;
	std::thread** Handlers = nullptr;
	std::queue<ParseFromSearch> VacParseInfo;
	std::mutex VacIdsMtx;
	std::counting_semaphore<> VacIdsSem{ 0 };
	std::queue<ExtraInfo> VacLoadArgs;
	std::mutex VacLoadArgsMtx;
	std::string SearchUrl = "https://api.hh.ru/vacancies?area=113";
	std::string VacUrl = "https://api.hh.ru/vacancies/";
	bool Active = true;
	bool Inited = false;
	int MaxValue = 0;
	int MaxValueHandlers = 1;
	int Period = 1;
public:
	explicit VacLoader(QVariantList& RolesList, int Period, QObject* Parent = nullptr);
	~VacLoader() override;
	void Load();
	void JoinNetwork(Network* Net);
	void JoinQueryHandler(QueryHandler* Query);
	void Stop();
signals:
	void ChangedVal(int Value, int MaxValue);
	void ChangedValHandle(int ValueHandle, int MaxValueHandle);
private:
	void ProviderWork(const int& ProviderId);
	void HandlerWork(const int& HandlerId);
	ExtraInfo GetVacLoadArgsItem();
	ParseFromSearch GetVacId();
	void PutVacId(ParseFromSearch Id);
	bool ParseVacIdsFromJson(const QJsonDocument& JSONParsed, ExtraInfo Args);
	Vacancy ParseVacancy(const QJsonDocument& JSONParsed);
	bool IsEnd();
};
