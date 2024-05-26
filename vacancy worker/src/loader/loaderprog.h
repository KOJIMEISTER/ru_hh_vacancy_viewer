#pragma once
#include <QObject>
#include "network.h"
#include "queryhandler.h"
#include "vacloader.h"
#include "vacverifier.h"

class LoaderProg : public QObject
{
	Q_OBJECT
	Network* Net = nullptr;
	QueryHandler* Query = nullptr;
	VacLoader* Loader = nullptr;
	VacVerifier* Verifier = nullptr;
	QVariantList RolesList;
	bool IsInited = false;
	bool Active = true;
	int Period = 1;
public:
	explicit LoaderProg(QVariantList RolesList, int Period, QObject* Parent = nullptr);
	~LoaderProg() override;
	void Start();
	void Stop();
	void Init();
signals:
	void StageChanged(QString Stage);
	void ValueChanged(int Value, int MaxValue);
	void ValueChangedHandle(int ValueHandle, int MaxValueHandle);
};

