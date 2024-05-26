#pragma once
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QObject>
#include "network.h"
#include "queryhandler.h"

class VacVerifier : public QObject
{
	Q_OBJECT
	Network* Net = nullptr;
	QueryHandler* Query = nullptr;
	int AmmHandlers = 1;
	bool* ActiveThreads = nullptr;
	std::thread** Handlers = nullptr;
	std::queue<unsigned int> VacIds;
	std::mutex VacIdsMtx;
	std::counting_semaphore<> VacIdsSem{ 0 };
	std::string VacUrl = "https://api.hh.ru/vacancies/";
	QVariantList RolesList;
	bool Active = true;
	bool Inited = false;
	int MaxValue = 0;
public:
	explicit VacVerifier(QVariantList& RolesList, QObject* Parent = nullptr);
	~VacVerifier() override;
	void Verify();
	void JoinNetwork(Network* Net);
	void JoinQueryHandler(QueryHandler* Query);
	void Stop();
signals:
	void ChangedVal(int Value, int MaxValue);
private:
	void HandlerWork(const int& HandlerId);
	unsigned int GetVacId();
	void PutVacId(const int& Id);
	void InitIds();
	bool IsEnd();
};

