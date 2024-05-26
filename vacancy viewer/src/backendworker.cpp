#include "backendworker.h"

BackendWorker::BackendWorker(QObject *parent)
	: QObject{parent}
{
	Net = new RemoteDatabase("RemoteDBWorker", this);
}

BackendWorker::~BackendWorker()
{
	Stop();
	delete Net;
}

void BackendWorker::Stop()
{
	StoppingState = true;
	SemaphoreVacPreviewsToLoad.release(9999);
}

void BackendWorker::JoinNet(RemoteDatabase* Net)
{
	this->Net = Net;
}

void BackendWorker::makeVacPreviewInfoRequest(int VacId, int Index)
{
	PutPairToLoadVacPreviewStack({VacId, Index});
}

QVariantMap BackendWorker::getVacPreviewInfo(int VacId)
{
	QVariantMap VacInfo;
	QSqlQuery Query = Net->GetVacancyPreview(VacId);
	if(Query.next())
	{
		VacInfo.insert("vacId", VacId);
		VacInfo.insert("vacName", Query.value("vac_name").toString());
		VacInfo.insert("paymentFrom", Query.value("vac_pay_from").toString());
		VacInfo.insert("paymentTo", Query.value("vac_pay_to").toString());
		VacInfo.insert("paymentCur", Query.value("vac_pay_cur").toString());
		VacInfo.insert("paymentGross", Query.value("vac_pay_gross").toBool());
		VacInfo.insert("area", Query.value("vac_area").toString());
		VacInfo.insert("schedule", Query.value("vac_schedule").toString());
		VacInfo.insert("experience", Query.value("vac_exp").toString());
		VacInfo.insert("employment", Query.value("vac_employment").toString());
		VacInfo.insert("roleId", Query.value("vac_role_id").toString());
	}
	Query.clear();
	return VacInfo;
}

QPair<int,int> BackendWorker::GetPairFromLoadVacPreviewStack()
{
	QPair<int,int> Pair;
	MutexVacPreivewStack.lock();
	Pair = LoadVacPreviewStack.pop();
	MutexVacPreivewStack.unlock();
	return Pair;
}

void BackendWorker::PutPairToLoadVacPreviewStack(QPair<int,int> Pair)
{
	MutexVacPreivewStack.lock();
	LoadVacPreviewStack.push(Pair);
	SemaphoreVacPreviewsToLoad.release();
	MutexVacPreivewStack.unlock();
}

void BackendWorker::LoadVacPreviewWorkerFunc()
{
	while(true)
	{
		SemaphoreVacPreviewsToLoad.acquire();
		if(StoppingState)
		{
			return;
		}
		QPair<int,int> Pair = GetPairFromLoadVacPreviewStack();
		QVariantMap VacPreview =  getVacPreviewInfo(Pair.first);
		emit sendVacPreview(VacPreview, Pair.second);
	}
}
