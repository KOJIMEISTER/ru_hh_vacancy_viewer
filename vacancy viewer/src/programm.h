#ifndef PROGRAMM_H
#define PROGRAMM_H

#include <QObject>
#include <QWidget>
#include "backend.h"
#include "backendworker.h"
#include "database.h"
#include "classificator.h"

class Programm : public QObject
{
	Q_OBJECT
	QThread BackendWorkerThread;
	QThread ClassifierThread;
	Backend* Back = nullptr;
	BackendWorker* BackWorker = nullptr;
	Database* DB = nullptr;
	Classificator* Classifier = nullptr;
public:
	explicit Programm(QObject *parent = nullptr);
	~Programm() override;
	Backend* GetBackend();
};

#endif // PROGRAMM_H
