#ifndef RemoteDatabase_H
#define RemoteDatabase_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

class RemoteDatabase : public QObject
{
	Q_OBJECT

	QString ConnName = "";
	QString ServerIp = "";
	int ServerPort = 5432;
	QString ServerPass = "";
	QString DatabaseName = "";
	QString UserName = "";
	QSqlDatabase DB;

public:

	explicit RemoteDatabase(QString ConnName, QObject *parent = nullptr);

	QSqlQuery GetVacancyPreview(int Id);
	QString GetVacancyRole(int Id);
	QSqlQuery GetVacancySearch(QString Params, bool Order);
	QList<QPair<QString, int>> GetSimillarTags(QString Tag);
	QList<QPair<QString, int>> GetSimillarCities(QString City);
	QList<QPair<QString, int>> GetAllRoles();
	QString GetVacancyDescrition(int Id);
	QList<QString> GetVacancyTags(int Id);

private:

	QSqlQuery select(QString StringQuery);
	QString PrepareFlags(QString Params);
	QString PrepareTags(QString Params);
	QString PrepareRoles(QString Params);
	QString PrepareCities(QString Params);
	void LoadConfig();

};

#endif // RemoteDatabase_H
