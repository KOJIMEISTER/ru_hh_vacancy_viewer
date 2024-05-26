#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <QObject>
#include <QFile>
#include <QDebug>
#include <QPair>
#include <QString>
#include <QSemaphore>
#include <QStack>
#include <QMutex>
#include <QList>
#include <QVariantList>
#undef slots
#include "boost/python.hpp"
using namespace boost::python;

class Interpreter : public QObject
{
	Q_OBJECT
	object moduleMain;
	object moduleDict;
	object& modMainInst();
	object& modDictInst();
	bool IsInited = false;
	QVariantList RolesList;
	QString ModelPath;
	QString SavePath;
	int Batches;
	int Epochs;
	PyThreadState *mainstate = nullptr;
public:
	explicit Interpreter(QObject* Parent = nullptr);
	~Interpreter() override;
	void doTrain();
	void PreInit(QVariantList RolesList, QString ModelPath, QString SavePath, int Batches, int Epochs);
	void Shutdown();
signals:
	void trainFinished();
private:
	void Init();
};

#endif // INTERPRETER_H
