#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QEventLoop>
#include <QList>
#include <QVariantMap>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QStack>
#include <QMutex>
#include <QPair>
#include <QSemaphore>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QCryptographicHash>
#include <QBitArray>
#include "backendworker.h"
#include "database.h"
#include "remotedatabase.h"
#include "classificator.h"

class Backend : public QObject
{
	Q_OBJECT

	QBitArray ViewedVacIds;
	BackendWorker* Worker = nullptr;
	Database* DB = nullptr;
	RemoteDatabase* Net = nullptr;
	Classificator* Classifier = nullptr;

public:

	explicit Backend(QObject *parent = nullptr);
	~Backend() override;

	Q_INVOKABLE QVariantList getVacToPrepareBySearch(QString Params, bool LoadViewed, bool Order);
	Q_INVOKABLE QVariantList getSimilarTags(QString Tag);
	Q_INVOKABLE QVariantList getSimilarCities(QString City);
	Q_INVOKABLE QVariantList getAllRoles();
	Q_INVOKABLE QString getVacancyDescription(int VacId);
	Q_INVOKABLE QVariantList getVacancyTags(int VacId);
	Q_INVOKABLE void clearAllViews();
	Q_INVOKABLE QVariantList getSearchHistories();
	Q_INVOKABLE void deleteHistory(int HistId);
	Q_INVOKABLE void updateHistoryViewedCount(int HistId);
	Q_INVOKABLE void renameHistory(QString Name, int HistId);
	Q_INVOKABLE QVariantMap jsonParamsToVarList(QString Params);
	Q_INVOKABLE QString getCurrentProfile();
	Q_INVOKABLE void setCurrentProfile(QString Name);
	Q_INVOKABLE QVariantList getAllProfiles();
	Q_INVOKABLE void deleteProfile(QString Name);
	Q_INVOKABLE void addProfile(QString Name);
	Q_INVOKABLE int getViewedVacsCount();
	void JoinWoker(BackendWorker* Worker);
	void JoinDatabase(Database* DB);
	void JoinClassifier(Classificator* Classifier);

signals:

	void sendVacPreview(QVariantMap VacPreiview, int Index);
	void makeVacPreviewInfoRequest(int VacId, int Index);
	void sendVacPredict(int Index, QString Predict);
	void vacOpenned(int VacId);

public Q_SLOTS:

	void ProxyRequest(int VacId, int Index);
	void makeVacToPredictRequest(int Index, int VacId, int RoleId);
	Q_INVOKABLE void updateVacViewed(QVariantMap VacPreiview, int Index);

private:

	QString ReformateText(QString TextToFormate);
	void PutPairToLoadVacPreviewStack(QPair<int,int> Pair);
	bool VacIsViewed(int VacId);
	void PutSearch(QString Params);
};

#endif // BACKEND_H
