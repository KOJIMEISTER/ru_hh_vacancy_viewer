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
#include <QFile>
#include <QMutex>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

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
	void SetIp(QString Ip);
	QString GetVacancyRole(int Id);
	QSqlQuery GetAllRoles();
	QString GetVacancyDescrition(int Id);
	QList<QString> GetVacancyTags(int Id);
	bool AddDir(QString Name, int RoleId);
	QSqlQuery GetDirs(int RoleId);
	void SetVacSkipped(int VacId);
	void AddVacToDataset(int VacId, QString Name);
	QSqlQuery GetVacancyToClassify(int RoleId);
	QSqlQuery GetVacancyTechs(int VacId);
	void ClearVacLabel(int VacId);
	QSqlQuery GetDatasetRoles();
	void DeleteDatasetRole(int RoleId);
	QSqlQuery GetAllRoleDirs();
	void DeleteRoleDir(QString DirName);
	QString GetVacCount();
private:
	void LoadConfig();
	QSqlQuery select(QString StringQuery);
	bool insert(QString StringQuery);
};

#endif // RemoteDatabase_H
