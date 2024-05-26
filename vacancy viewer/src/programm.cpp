#include "programm.h"

Programm::Programm(QObject *parent)
	: QObject{parent}
{
	DB = new Database();
	BackWorker = new BackendWorker();
	Back = new Backend();
	Classifier = new Classificator();
	Back->JoinDatabase(DB);
	Back->JoinWoker(BackWorker);
	Back->JoinClassifier(Classifier);
	connect(&BackendWorkerThread, &QThread::started, BackWorker, &BackendWorker::LoadVacPreviewWorkerFunc);
	connect(&BackendWorkerThread, &QThread::started, BackWorker, &QObject::deleteLater);
	BackWorker->moveToThread(&BackendWorkerThread);
	connect(&ClassifierThread, &QThread::started, Classifier, &Classificator::RecieveProcess);
	connect(&ClassifierThread, &QThread::started, Classifier, &QObject::deleteLater);
	Classifier->moveToThread(&ClassifierThread);
	BackendWorkerThread.start();
	ClassifierThread.start();
}

Programm::~Programm()
{
	BackWorker->Stop();
	Classifier->Stop();
	BackendWorkerThread.quit();
	BackendWorkerThread.wait();
	ClassifierThread.quit();
	ClassifierThread.wait();
	delete Back;
	delete DB;
}

Backend* Programm::GetBackend()
{
	return Back;
}
