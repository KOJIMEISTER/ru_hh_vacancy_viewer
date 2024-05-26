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

QString RemoteDatabase::GetVacCount()
{
	QString Number = "0";
	QSqlQuery Query(DB);
	Query.exec("select count(*) from vacancy");
	if(Query.lastError().isValid())
	{
		qDebug() << Query.lastError();
	}
	else
	{
		Query.next();
		Number = Query.value("count").toString();
	}
	Query.clear();
	return Number;
}

void RemoteDatabase::DeleteRoleDir(QString DirName)
{
	insert(QString("delete from vacancy_types where type_name='%1'").arg(DirName));
}

QSqlQuery RemoteDatabase::GetAllRoleDirs()
{
	QSqlQuery Query(DB);
	Query.exec("select type_name, count(*) from vacancy_dataset group by type_name order by count(*) desc");
	if(Query.lastError().isValid())
	{
		qDebug() << Query.lastError();
	}
	return Query;
}

void RemoteDatabase::DeleteDatasetRole(int RoleId)
{
	insert(QString("delete from vacancy_dataset where type_name in (select type_name from vacancy_types where role_id = '%1')").arg(RoleId));
}

QSqlQuery RemoteDatabase::GetDatasetRoles()
{
	QSqlQuery Query(DB);
	Query.exec("select * from all_dataset_roles");
	return Query;
}

void RemoteDatabase::ClearVacLabel(int VacId)
{
	insert(QString("delete from vacancy_dataset where vac_id = '%1'").arg(VacId));
}

QSqlQuery RemoteDatabase::GetVacancyToClassify(int RoleId)
{
	QSqlQuery Query(DB);
	Query.exec(QString("select vac_id, vac_description, vac_name from vacancy where vac_id not in (select vac_id from vacancy_dataset) and vac_id not in (select vac_id from vacancy_classify_error) and vac_role_id = '%1' limit 1").arg(RoleId));
	return Query;
}

QSqlQuery RemoteDatabase::GetVacancyTechs(int VacId)
{
	QSqlQuery Query(DB);
	Query.exec(QString("select tech_name from vacancy_techs where vac_id = '%1'").arg(VacId));
	return Query;
}

void RemoteDatabase::AddVacToDataset(int VacId, QString Name)
{
	insert(QString("insert into vacancy_dataset values ('%1','%2')").arg(VacId).arg(Name));
}

void RemoteDatabase::SetVacSkipped(int VacId)
{
	insert(QString("insert into vacancy_classify_error values ('%1')").arg(VacId));
}

QSqlQuery RemoteDatabase::GetDirs(int RoleId)
{
	QSqlQuery Query(DB);
	Query.exec(QString("select * from vacancy_types where role_id = '%1'").arg(RoleId));
	if(Query.lastError().isValid())
	{
		qDebug() << Query.lastError();
	}
	return Query;
}

bool RemoteDatabase::AddDir(QString Name, int RoleId)
{
	QString StringQuery = QString("insert into vacancy_types values ('%1', '%2')").arg(Name).arg(RoleId);
	return insert(StringQuery);
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

QSqlQuery RemoteDatabase::GetAllRoles()
{
	QSqlQuery Query(DB);
	Query.exec("select * from all_roles");
	if(Query.lastError().isValid())
	{
		qDebug() << Query.lastError();
	}
	return Query;
}

QString RemoteDatabase::GetVacancyDescrition(int Id)
{
	QString Description;
	QSqlQuery Query(DB);
	Query.exec(QString("select vac_description from vacancy where vac_id = '%1'").arg(Id));
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

void RemoteDatabase::SetIp(QString Ip)
{
	ServerIp = Ip;
	DB.close();
	DB.setHostName(ServerIp);
	DB.open();
}

bool RemoteDatabase::insert(QString StringQuery)
{
	QSqlQuery Query(DB);
	Query.exec(StringQuery);
	if(Query.lastError().isValid())
	{
		qDebug() << Query.lastError();
		Query.clear();
		return false;
	}
	Query.clear();
	return true;
}
