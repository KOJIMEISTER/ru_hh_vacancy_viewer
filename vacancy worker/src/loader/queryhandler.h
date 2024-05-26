#pragma once
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QFile>
#include <iostream>
#include <unordered_map>
#include <bitset>
#include <mutex>
#include <queue>
#include <regex>
#include "vacancy.hpp"

class QueryHandler
{
	std::bitset<134217728> IdsTable;
	std::mutex MtxIdsTable;
	QSqlDatabase DB;
	std::mutex MtxDBQuery;
	QString ConnName = "";
	QString ServerIp = "";
	int ServerPort = 0;
	QString ServerPass = "";
	QString DatabaseName = "";
	QString UserName = "";
public:
	QueryHandler(QString ConnName);
	void AddVacancyToDB(const Vacancy& Vac);
	void UpdateArchive(const Vacancy& Vac);
	bool IdsExists(const unsigned int& Id);
	bool PutIdsTable(const unsigned int& Id);
	std::list<std::unordered_map<std::string, std::string>> DoSelect(std::string SelectMsg);
	int DoInsert(std::string InsertMsg);
	void HashAndClear();
private:
	void LoadConfig();
	void InitIdsTable();
	std::string VacToStr(const Vacancy& Vac);
	std::string KeyTechsToStr(const Vacancy& Vac);
	std::string Escaping(std::string ToEscape);
	std::string PrepareVal(std::string ToPrepare);
};

