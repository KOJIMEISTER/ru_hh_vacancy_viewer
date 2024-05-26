#include "database.h"

Database::Database(QObject *parent)
	: QObject{parent}
{
	DB = QSqlDatabase::addDatabase("QSQLITE", "LocalDBConnection");
	DB.setDatabaseName("db.db");
	DB.open();
	initProfiles();
	initAppProperties();
	initVacancies();
	initSearchesHistories();
}

void Database::initProfiles()
{
	QSqlQuery Query(DB);
	Query.exec("select name from sqlite_master where type='table' and name='profiles'");
	if(Query.lastError().isValid())
	{
		qDebug() << Query.lastError();
	}
	else if(!Query.next())
	{
		Query.clear();
		Query.exec("create table profiles"
			   "(profile_name varchar(64) primary key)");
		if(Query.lastError().isValid())
		{
			qDebug() << Query.lastError();
		}
		Query.clear();
		commit();
		initBaseProfile();
	}
	else
	{
		Query.clear();
	}
}

void Database::initBaseProfile()
{
	QSqlQuery Query(DB);
	Query.exec("insert into profiles (profile_name) values ('Базовый')");
	if(Query.lastError().isValid())
	{
		qDebug() << Query.lastError();
	}
	Query.clear();
	commit();
}

void Database::initAppPropertiesDefaults()
{
	insert("insert into app_properties (property_name, property_value) values ('server_ip', 'localhost'), ('current_profile', 'Базовый')");
	commit();
}

void Database::initAppProperties()
{
	QSqlQuery Query(DB);
	Query.exec("select name from sqlite_master where type='table' and name='app_properties'");
	if(Query.lastError().isValid())
	{
		qDebug() << Query.lastError();
	}
	else if(!Query.next())
	{
		Query.clear();
		Query.exec("create table app_properties"
			   "(property_name varchar(64) primary key, "
			   "property_value varchar(64))");
		if(Query.lastError().isValid())
		{
			qDebug() << Query.lastError();
		}
		Query.clear();
		commit();
		initAppPropertiesDefaults();
	}
	else
	{
		Query.clear();
	}
}

void Database::initVacancies()
{
	QSqlQuery Query(DB);
	Query.exec("select name from sqlite_master where type='table' and name='vacancies'");
	if(Query.lastError().isValid())
	{
		qDebug() << Query.lastError();
	}
	else if(!Query.next())
	{
		Query.clear();
		Query.exec("create table vacancies"
			   "(vac_id bigint, "
			   "vac_viewed_at bigint, "
			   "by_profile varchar(64))");
		if(Query.lastError().isValid())
		{
			qDebug() << Query.lastError();
		}
		Query.clear();
		commit();
	}
	else
	{
		Query.clear();
	}
}

void Database::initSearchesHistories()
{
	QSqlQuery Query(DB);
	Query.exec("select name from sqlite_master where type='table' and name='histories'");
	if(Query.lastError().isValid())
	{
		qDebug() << Query.lastError();
	}
	else if(!Query.next())
	{
		Query.clear();
		Query.exec("create table histories"
			   "(history_id integer primary key, "
			   "history_name varchar(64) default ('Безымянный'), "
			   "history_params varchar(1024), "
			   "history_hash varchar(64),"
			   "history_found integer default(0), "
			   "history_watched integer default(0),"
			   "by_profile varchar(64))");
		if(Query.lastError().isValid())
		{
			qDebug() << Query.lastError();
		}
		Query.clear();
		commit();
	}
	else
	{
		Query.clear();
	}
}

void Database::commit()
{
	MutexDB.lock();
	DB.commit();
	MutexDB.unlock();
}

QSqlQuery Database::select(QString StringQuery)
{
	QSqlQuery Query(DB);
	Query.exec(StringQuery);
	if(Query.lastError().isValid())
	{
		qDebug() << Query.lastError();
	}
	return Query;
}

bool Database::insert(QString StringQuery)
{
	QSqlQuery Query(DB);
	Query.exec(StringQuery);
	DB.commit();
	if(Query.lastError().isValid())
	{
		qDebug() << Query.lastError();
		Query.clear();
		return false;
	}
	else
	{
		Query.clear();
		return true;
	}
}
