#include "queryhandler.h"

QueryHandler::QueryHandler(QString ConnName)
{
	LoadConfig();
	this->ConnName = ConnName;
	DB = QSqlDatabase::addDatabase("QPSQL", this->ConnName);
	DB.setHostName(ServerIp);
	DB.setUserName(UserName);
	DB.setPort(ServerPort);
	DB.setDatabaseName(DatabaseName);
	DB.setPassword(ServerPass);
	DB.open();
	if(!DB.isOpen())
	{
		qDebug() << "Remote connect to database server is failed : " << DB.lastError().text();
	}
	else
	{
		InitIdsTable();
	}
}

void QueryHandler::LoadConfig()
{
	QFile Config("config.json");
	if(Config.open(QFile::ReadOnly))
	{
		QJsonDocument Json = QJsonDocument::fromJson(Config.readAll());
		Config.close();
		ServerIp = Json["host"].toString();
		ServerPort = Json["port"].toInt();
		ServerPass = Json["pass"].toString();
		DatabaseName = Json["dbname"].toString();
		UserName = Json["user"].toString();
	}
}

void QueryHandler::InitIdsTable()
{
	auto Result = DoSelect("select vac_id from all_vacancies");
	for (auto& Row : Result)
	{
		unsigned int Id = std::stoi(Row.at("vac_id"));
		IdsTable.set(Id, 1);
	}
}

bool QueryHandler::PutIdsTable(const unsigned int& Id)
{
	bool IsExists = false;
	MtxIdsTable.lock();
	IsExists = IdsTable.test(Id);
	IdsTable.set(Id, 1);
	MtxIdsTable.unlock();
	return IsExists;
}

bool QueryHandler::IdsExists(const unsigned int& Id)
{
	bool IsExists = false;
	MtxIdsTable.lock();
	IsExists = IdsTable.test(Id);
	MtxIdsTable.unlock();
	return IsExists;
}

std::string QueryHandler::Escaping(std::string ToEscape)
{
	return std::regex_replace(ToEscape, std::regex("'"), "''");
}

std::string QueryHandler::PrepareVal(std::string ToPrepare)
{
	std::string Prepared;
	Prepared.reserve(64);
	Prepared.append("'");
	Prepared.append(Escaping(ToPrepare));
	Prepared.append("'");
	return Prepared;
}

std::string QueryHandler::VacToStr(const Vacancy& Vac)
{
	std::string StrVac = "(";
	StrVac.reserve(16364);
	StrVac.append(PrepareVal(Vac.VacancyId));
	StrVac.append(",");
	StrVac.append(PrepareVal(Vac.VacancyName));
	StrVac.append(",");
	StrVac.append(PrepareVal(Vac.CompanyName));
	StrVac.append(",");
	StrVac.append(PrepareVal(Vac.PaymentFrom));
	StrVac.append(",");
	StrVac.append(PrepareVal(Vac.PaymentTo));
	StrVac.append(",");
	StrVac.append(PrepareVal(Vac.PaymentCur));
	StrVac.append(",");
	StrVac.append(PrepareVal(Vac.PaymentGross ? "false" : "true"));
	StrVac.append(",");
	StrVac.append(PrepareVal(Vac.Area));
	StrVac.append(",");
	StrVac.append(PrepareVal("false"));
	StrVac.append(",");
	StrVac.append(PrepareVal(Vac.WorkExp));
	StrVac.append(",");
	StrVac.append(PrepareVal(Vac.Schedule));
	StrVac.append(",");
	StrVac.append(PrepareVal(Vac.Employment));
	StrVac.append(",");
	StrVac.append(PrepareVal(Vac.Description));
	StrVac.append(",");
	StrVac.append(PrepareVal(Vac.Published));
	StrVac.append(",");
	StrVac.append(PrepareVal(Vac.Role));
	StrVac.append(")");
	return StrVac;
}

std::string QueryHandler::KeyTechsToStr(const Vacancy& Vac)
{
	std::string StrKeyTechs = "";
	auto Iter = Vac.KeySkills.begin();
	if (Iter != Vac.KeySkills.end())
	{
		while (true)
		{
			StrKeyTechs.append("(");
			StrKeyTechs.append(PrepareVal(Vac.VacancyId));
			StrKeyTechs.append(",");
			StrKeyTechs.append(PrepareVal((*Iter)));
			StrKeyTechs.append(")");
			++Iter;
			if (Iter == Vac.KeySkills.end())
			{
				break;
			}
			else
			{
				StrKeyTechs.append(",");
			}
		}
	}
	return StrKeyTechs;
}

void QueryHandler::AddVacancyToDB(const Vacancy& Vac)
{
	std::string Insert = "insert into vacancy (vac_id, vac_name, vac_company, vac_pay_from, vac_pay_to, vac_pay_cur, vac_pay_gross, vac_area, vac_archived, vac_exp, vac_schedule, vac_employment, vac_description, vac_published, vac_role_id) values ";
	std::string InsertKeyTechs = "insert into vacancy_techs (vac_id, tech_name) values ";
	Insert.append(VacToStr(Vac));
	DoInsert(Insert);
	if (!Vac.KeySkills.empty())
	{
		InsertKeyTechs.append(KeyTechsToStr(Vac));
		DoInsert(InsertKeyTechs);
	}
}

void QueryHandler::UpdateArchive(const Vacancy& Vac)
{
	std::string Update = "update vacancy set vac_archived = 'true' where vac_id =";
	Update.append(PrepareVal(Vac.VacancyId));
	DoInsert(Update);
}

void QueryHandler::HashAndClear()
{
	qDebug() << "\nStart set original and duplicated\n";
	DoInsert("call set_original_and_duplicated()");
}

std::list<std::unordered_map<std::string, std::string>> QueryHandler::DoSelect(std::string SelectMsg)
{
	std::list<std::unordered_map<std::string, std::string>> QueryRows;
	MtxDBQuery.lock();
	QSqlQuery Query(DB);
	Query.exec(SelectMsg.c_str());
	if(Query.lastError().isValid())
	{
		qDebug() << Query.lastError();
	}
	QSqlRecord QueryRec = Query.record();
	while (Query.next())
	{
		std::unordered_map<std::string, std::string> Map;
		for (int i = 0; i < QueryRec.count(); ++i)
		{
			std::string Name = QueryRec.fieldName(i).toStdString();
			std::string Value = Query.value(i).toString().toStdString();
			Map.insert({Name, Value});
		}
		QueryRows.push_back(Map);
	}
	Query.clear();
	MtxDBQuery.unlock();
	return QueryRows;
}

int QueryHandler::DoInsert(std::string InsertMsg)
{
	int Success = 1;
	MtxDBQuery.lock();
	QSqlQuery Query(DB);
	Query.exec(InsertMsg.c_str());
	if(Query.lastError().isValid())
	{
		Success = 0;
		qDebug() << Query.lastError();
	}
	Query.clear();
	MtxDBQuery.unlock();
	return Success;
}
