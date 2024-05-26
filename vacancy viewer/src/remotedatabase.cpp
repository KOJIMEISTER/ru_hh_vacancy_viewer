#include "remotedatabase.h"

RemoteDatabase::RemoteDatabase(QString ConnName, QObject *parent)
	: QObject{parent}
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
}

void RemoteDatabase::LoadConfig()
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

QSqlQuery RemoteDatabase::GetVacancyPreview(int Id)
{
	QSqlQuery Query(DB);
	Query.exec(QString("select vac_id, vac_name, vac_pay_from, vac_pay_to, vac_pay_cur, vac_pay_gross, vac_area, vac_exp, vac_schedule, vac_employment, vac_role_id from cleared_vacancies where vac_id = '%1'").arg(Id));
	return Query;
}

QString RemoteDatabase::GetVacancyRole(int Id)
{
	QString Role;
	QSqlQuery Query(DB);
	Query.exec(QString("select role_name from vacancy_roles where role_id in (select vac_role_id from vacancy where vac_id = '%1')").arg(Id));
	if(Query.next())
	{
		Role = Query.value(0).toString();
	}
	Query.clear();
	return Role;
}

QString RemoteDatabase::PrepareFlags(QString Params)
{
	QString QueryStr = "";
	int Count = 0;
	QJsonDocument Json = QJsonDocument::fromJson(Params.toUtf8());
	if(Json["shift"].toBool() or Json["remote"].toBool() or Json["agile"].toBool() or Json["fullday"].toBool())
	{
		QueryStr += "(";
	}
	if(Json["shift"].toBool())
	{
		QueryStr += "vac_schedule='shift' ";
		++Count;
	}
	if(Json["remote"].toBool())
	{
		if(Count != 0)
		{
			QueryStr += "or ";
		}
		QueryStr += "vac_schedule='remote' ";
		++Count;
	}
	if(Json["agile"].toBool())
	{
		if(Count != 0)
		{
			QueryStr += "or ";
		}
		QueryStr += "vac_schedule='flexible' ";
		++Count;
	}
	if(Json["fullday"].toBool())
	{
		if(Count != 0)
		{
			QueryStr += "or ";
		}
		QueryStr += "vac_schedule='fullDay' ";
		++Count;
	}
	if(Json["shift"].toBool() or Json["remote"].toBool() or Json["agile"].toBool() or Json["fullday"].toBool())
	{
		QueryStr += ")";
	}

	Count = 0;
	if(Json["from6"].toBool() or Json["from3to6"].toBool() or Json["from1to3"].toBool() or Json["unexp"].toBool())
	{
		if(Json["shift"].toBool() or Json["remote"].toBool() or Json["agile"].toBool() or Json["fullday"].toBool())
		{
			QueryStr += " and ";
		}
		QueryStr += "(";
	}
	if(Json["from6"].toBool())
	{
		if(Count != 0)
		{
			QueryStr += "or ";
		}
		QueryStr += "vac_exp='moreThan6' ";
		++Count;
	}
	if(Json["from3to6"].toBool())
	{
		if(Count != 0)
		{
			QueryStr += "or ";
		}
		QueryStr += "vac_exp='between3And6' ";
		++Count;
	}
	if(Json["from1to3"].toBool())
	{
		if(Count != 0)
		{
			QueryStr += "or ";
		}
		QueryStr += "vac_exp='between1And3' ";
		++Count;
	}
	if(Json["unexp"].toBool())
	{
		if(Count != 0)
		{
			QueryStr += "or ";
		}
		QueryStr += "vac_exp='noExperience' ";
		++Count;
	}
	if(Json["from6"].toBool() or Json["from3to6"].toBool() or Json["from1to3"].toBool() or Json["unexp"].toBool())
	{
		QueryStr += ")";
	}

	Count = 0;
	if(Json["intern"].toBool() or Json["parttime"].toBool() or Json["parttime"].toBool() or Json["fulltime"].toBool())
	{
		if(Json["shift"].toBool() or Json["remote"].toBool() or Json["agile"].toBool() or Json["fullday"].toBool() or Json["from6"].toBool() or Json["from3to6"].toBool() or Json["from1to3"].toBool() or Json["unexp"].toBool())
		{
			QueryStr += " and ";
		}
		QueryStr += "(";
	}
	if(Json["intern"].toBool())
	{
		if(Count != 0)
		{
			QueryStr += "or ";
		}
		QueryStr += "vac_employment='probation' ";
		++Count;
	}
	if(Json["parttime"].toBool())
	{
		if(Count != 0)
		{
			QueryStr += "or ";
		}
		QueryStr += "vac_employment='part' ";
		++Count;
	}
	if(Json["fulltime"].toBool())
	{
		if(Count != 0)
		{
			QueryStr += "or ";
		}
		QueryStr += "vac_employment='full' ";
		++Count;
	}
	if(Json["intern"].toBool() or Json["parttime"].toBool() or Json["parttime"].toBool() or Json["fulltime"].toBool())
	{
		QueryStr += ")";
	}
	return QueryStr;
}

QString RemoteDatabase::PrepareTags(QString Params)
{
	QString QueryStr = "";
	QJsonDocument Json = QJsonDocument::fromJson(Params.toUtf8());
	if(!Json["tags"].toArray().isEmpty())
	{
		QueryStr += " vac_id in ";
		QString IterativeStr = "";
		QString Str = "";
		for(const auto Tag : Json["tags"].toArray())
		{
			Str = QString("(select vac_id from vacancy_techs where tech_name = '%1')").arg(Tag.toObject().value("name").toString());
			if(IterativeStr.isEmpty())
			{
				IterativeStr = Str;
			}
			else
			{
				if(Tag.toObject().value("type").toBool())
				{
					IterativeStr = "(select vac_id from " + IterativeStr + " where vac_id in " + Str + ")";
				}
				else
				{
					IterativeStr = "(select vac_id from " + IterativeStr + " where vac_id not in " + Str + ")";
				}
			}
		}
		QueryStr += IterativeStr;
	}
	return QueryStr;
}

QString RemoteDatabase::PrepareRoles(QString Params)
{
	QString QueryStr = "";
	QJsonDocument Json = QJsonDocument::fromJson(Params.toUtf8());
	if(!Json["roles"].toArray().isEmpty())
	{
		QueryStr += " vac_id in (select vac_id from vacancy where ";
		int Count = 0;
		for(auto const role : Json["roles"].toArray())
		{
			if(Count != 0)
			{
				QueryStr += " or ";
			}
			QueryStr += QString("vac_role_id in (select role_id from vacancy_roles where role_name='%1')").arg(role.toObject().value("name").toString());
			++Count;
		}
		QueryStr += " group by vac_id)";
	}
	return QueryStr;
}

QString RemoteDatabase::PrepareCities(QString Params)
{
	QString QueryStr = "";
	QJsonDocument Json = QJsonDocument::fromJson(Params.toUtf8());
	if(!Json["cities"].toArray().isEmpty())
	{
		QueryStr += " vac_id in (select vac_id from cleared_vacancies where ";
		int Count = 0;
		for(const auto City : Json["cities"].toArray())
		{
			if(Count != 0)
			{
				QueryStr += " or ";
			}
			QueryStr += QString("(vac_area = '%1')").arg(City.toObject().value("name").toString());
			++Count;
		}
		QueryStr += ")";
	}
	return QueryStr;
}

QSqlQuery RemoteDatabase::GetVacancySearch(QString Params, bool Order)
{
	QString QueryStr = "select vac_id from cleared_vacancies";
	QString FlagsStr = PrepareFlags(Params);
	QString TagsStr = PrepareTags(Params);
	QString RoleStr = PrepareRoles(Params);
	QString CitiesStr = PrepareCities(Params);
	if(!FlagsStr.isEmpty() or !TagsStr.isEmpty() or !RoleStr.isEmpty() or !CitiesStr.isEmpty())
	{
		QueryStr += " where";
	}
	if(!FlagsStr.isEmpty())
	{
		QueryStr += " (" + FlagsStr + ")";
	}
	if(!TagsStr.isEmpty())
	{
		if(!FlagsStr.isEmpty())
		{
			QueryStr += " and " + TagsStr;
		}
		else
		{
			QueryStr += " " + TagsStr;
		}
	}
	if(!RoleStr.isEmpty())
	{
		if(!FlagsStr.isEmpty() or !TagsStr.isEmpty())
		{
			QueryStr += " and " + RoleStr;
		}
		else
		{
			QueryStr += " " + RoleStr;
		}
	}
	if(!CitiesStr.isEmpty())
	{
		if(!FlagsStr.isEmpty() or !TagsStr.isEmpty() or !RoleStr.isEmpty())
		{
			QueryStr += " and " + CitiesStr;
		}
		else
		{
			QueryStr += " " + CitiesStr;
		}
	}
	if(Order)
	{
		QueryStr += " order by vac_published desc";
	}
	else
	{
		QueryStr += " order by vac_published asc";
	}
	QSqlQuery Query(DB);
	Query.exec(QueryStr);
	return Query;
}

QList<QPair<QString, int>> RemoteDatabase::GetSimillarTags(QString Tag)
{
	QList<QPair<QString, int>> ListSpec;
	QSqlQuery Query(DB);
	Query.exec(QString("select tech_name, count from all_techs where tech_name like '%1%' order by count desc limit 5").arg(Tag));
	while(Query.next())
	{
		ListSpec.push_back({Query.value("tech_name").toString(), Query.value("count").toInt()});
	}
	Query.clear();
	return ListSpec;
}

QList<QPair<QString, int>> RemoteDatabase::GetSimillarCities(QString City)
{
	QList<QPair<QString, int>> ListSpec;
	QSqlQuery Query(DB);
	Query.exec(QString("select * from all_cities where vac_area like '%1%' order by count desc limit 5").arg(City));
	while(Query.next())
	{
		ListSpec.push_back({Query.value(0).toString(), Query.value(1).toInt()});
	}
	Query.clear();
	return ListSpec;
}

QList<QPair<QString, int>> RemoteDatabase::GetAllRoles()
{
	QList<QPair<QString, int>> ListSpec;
	QSqlQuery Query(DB);
	Query.exec("select * from all_roles");
	while(Query.next())
	{
		ListSpec.push_back({Query.value("role_name").toString(), Query.value("count").toInt()});
	}
	Query.clear();
	return ListSpec;
}

QString RemoteDatabase::GetVacancyDescrition(int Id)
{
	QString Description;
	QSqlQuery Query(DB);
	Query.exec(QString("select vac_description from cleared_vacancies where vac_id = '%1'").arg(Id));
	if(Query.next())
	{
		Description = Query.value(0).toString();
	}
	Query.clear();
	return Description;
}

QList<QString> RemoteDatabase::GetVacancyTags(int Id)
{
	QList<QString> Tags;
	QSqlQuery Query(DB);
	Query.exec(QString("select tech_name from vacancy_techs where vac_id = '%1'").arg(Id));
	while(Query.next())
	{
		Tags.push_back(Query.value(0).toString());
	}
	Query.clear();
	return Tags;
}

QSqlQuery RemoteDatabase::select(QString StringQuery)
{
	QSqlQuery Query(DB);
	Query.exec(StringQuery);
	if(Query.lastError().isValid())
	{
		qDebug() << Query.lastError();
	}
	return Query;
}
