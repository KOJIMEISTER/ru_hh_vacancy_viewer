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
#include <QThread>
#include "remotedatabase.h"
#include "loader/loaderprog.h"
#include "interpreter/interpreter.h"

class Backend : public QObject
{
	Q_OBJECT

	RemoteDatabase* Net = nullptr;

	LoaderProg* Loader = nullptr;
	QThread LoaderThread;

	Interpreter* Trainer = nullptr;
	QThread TrainerThread;

	QList<QVariantMap> HistoryVac;
	int CurIdx = 0;

	QString CurState = "Idle";
	int CurValue = 0;
	int CurValueHandle = 0;
	int MaxValue = 1;
	int MaxValueHandle = 1;
	bool IsTrainExecute = false;
	Q_PROPERTY(QString curState MEMBER CurState NOTIFY curStateChanged)
	Q_PROPERTY(int curValue MEMBER CurValue NOTIFY curValueChanged)
	Q_PROPERTY(int curValueHandle MEMBER CurValueHandle NOTIFY curValueHandleChanged)
	Q_PROPERTY(int maxValue MEMBER MaxValue NOTIFY maxValueChanged)
	Q_PROPERTY(int maxValueHandle MEMBER MaxValueHandle NOTIFY maxValueHandleChanged)
	Q_PROPERTY(bool isTrainExecute MEMBER IsTrainExecute NOTIFY isTrainExecuteChanged)

public:

	explicit Backend(QObject *parent = nullptr);
	~Backend() override;

	Q_INVOKABLE QVariantList getAllRoles();
	Q_INVOKABLE QString getVacancyDescription(int VacId);
	Q_INVOKABLE bool addDir(QString Name, int RoleId);
	Q_INVOKABLE QVariantList getDirs(int RoleId);
	Q_INVOKABLE void setVacSkipped(int VacId);
	Q_INVOKABLE void addVacToDataset(int VacId, QString Name);
	Q_INVOKABLE void previousVac();
	Q_INVOKABLE void nextVac(int RoleId);
	Q_INVOKABLE QVariantMap getCurrentVac();
	Q_INVOKABLE QString reformateText(QString TextToFormate);
	Q_INVOKABLE void clearVacLabel(int VacId);
	Q_INVOKABLE QVariantList getDatasetRoles();
	Q_INVOKABLE void deleteDatasetRole(int RoleId);
	Q_INVOKABLE QVariantList getAllRoleDirs();
	Q_INVOKABLE void deleteRoleDir(QString DirName);
	Q_INVOKABLE void clearHistory();
	Q_INVOKABLE QString getVacCount();
	Q_INVOKABLE void refreshVacs(QVariantList RolesList, int Period);
	Q_INVOKABLE void stopRefresh();
	Q_INVOKABLE void startTrain(QVariantList RolesList, QString ModelPath, QString SavePath, int Batches, int Epochs);

signals:

	void curStateChanged();
	void curValueChanged();
	void curValueHandleChanged();
	void maxValueChanged();
	void maxValueHandleChanged();
	void isTrainExecuteChanged();

public Q_SLOTS:

	void getChangedState(QString Stage);
	void getChangedValue(int Value, int MaxValue);
	void getChangedValueHandle(int ValueHandle, int MaxValueHandle);
	void toIdleAfterThreadDone();
	void trainFinished();

};

#endif // BACKEND_H
