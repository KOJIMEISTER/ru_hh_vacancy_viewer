#ifndef CLASSIFICATOR_H
#define CLASSIFICATOR_H
#include <QObject>
#include <QFile>
#include <QDebug>
#include <QPair>
#include <QString>
#include <QSemaphore>
#include <QStack>
#include <QMutex>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#undef slots
#include "boost/python.hpp"
using namespace boost::python;

class Classificator : public QObject
{
	Q_OBJECT
	object moduleMain;
	object moduleDict;
	object& modMainInst();
	object& modDictInst();
	bool StoppingState = false;
	QSemaphore SemToPredict;
	QMutex MutexToPredict;
	QStack<QPair<int, QString>> StackToPredict;
	void PutToPredict(int Index, QString Text);
	QPair<int, QString> GetToPredict();
	bool IsRoleIdAllow(int RoleId);
	void LoadConfig();
	QList<int> AllowedRoleIdList;
	int MinAccuracy = 50;
public:
	explicit Classificator(QObject* Parent = nullptr);
	~Classificator() override;
	QPair<QString, int> doClassification(QString Text);
signals:
	void sendPredict(int Index, QString Predict);
public Q_SLOTS:
	void Stop();
	void RecieveProcess();
	void RequestPredict(int Index, QString Text, int RoleId);
};

#endif // CLASSIFICATOR_H
