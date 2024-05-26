#ifndef BACKENDWORKER_H
#define BACKENDWORKER_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QUrl>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QEventLoop>
#include <QList>
#include <QVariantMap>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QFuture>
#include <QStack>
#include <QEvent>
#include <QMutex>
#include <QPair>
#include <QSemaphore>
#include "remotedatabase.h"

class BackendWorker : public QObject
{
	Q_OBJECT
	QStack<QPair<int, int>> LoadVacPreviewStack;
	QMutex MutexVacPreivewStack;
	QSemaphore SemaphoreVacPreviewsToLoad;
	RemoteDatabase* Net = nullptr;
	bool StoppingState = false;
public:
	explicit BackendWorker(QObject *parent = nullptr);
	~BackendWorker() override;
	void JoinNet(RemoteDatabase* Net);
signals:
	void sendVacPreview(QVariantMap VacPreiview, int Index);
public slots:
	void LoadVacPreviewWorkerFunc();
	void makeVacPreviewInfoRequest(int VacId, int Index);
	void Stop();
private:
	QPair<int,int> GetPairFromLoadVacPreviewStack();
	void PutPairToLoadVacPreviewStack(QPair<int,int> Pair);
	QVariantMap getVacPreviewInfo(int VacId);
};

#endif // BACKENDWORKER_H
