#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QMutex>

class Database : public QObject
{
	Q_OBJECT
	QSqlDatabase DB;
	QMutex MutexDB;
public:
	explicit Database(QObject *parent = nullptr);
	void commit();
	QSqlQuery select(QString StringQuery);
	bool insert(QString StringQuery);
private:
	void initAppProperties();
	void initAppPropertiesDefaults();
	void initVacancies();
	void initSearchesHistories();
	void initProfiles();
	void initBaseProfile();
};

#endif // DATABASE_H
