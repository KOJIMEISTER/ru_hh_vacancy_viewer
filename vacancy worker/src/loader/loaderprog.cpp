#include "loaderprog.h"

LoaderProg::LoaderProg(QVariantList RolesList, int Period, QObject* Parent) : QObject{Parent}
{
	this->RolesList = RolesList;
	this->Period = Period;
}

LoaderProg::~LoaderProg()
{
	Stop();
	if(Verifier)
	{
		delete Verifier;
	}
	if(Loader)
	{
		delete Loader;
	}
	if(Query)
	{
		delete Query;
	}
	if(Net)
	{
		delete Net;
	}
}

void LoaderProg::Stop()
{
	Active = false;
	if(Verifier != nullptr)
	{
		Verifier->Stop();
	}
	if(Loader != nullptr)
	{
		Loader->Stop();
	}
	if(Net != nullptr)
	{
		Net->Stop();
	}
}

void LoaderProg::Start()
{
	if(!IsInited)
	{
		Init();
	}
	if(Active)
	{
		emit StageChanged("Validate");
		Verifier->Verify();
	}
	if(Active)
	{
		emit StageChanged("Load");
		Loader->Load();
	}
	if(Active)
	{
		emit StageChanged("Hash");
		Query->HashAndClear();
	}
	emit StageChanged("Stop");
	Stop();
}

void LoaderProg::Init()
{
	IsInited = true;
	Verifier = new VacVerifier(RolesList);
	connect(Verifier, &VacVerifier::ChangedVal, this, &LoaderProg::ValueChanged);
	Loader = new VacLoader(RolesList, Period);
	connect(Loader, &VacLoader::ChangedVal, this, &LoaderProg::ValueChanged);
	connect(Loader, &VacLoader::ChangedValHandle, this, &LoaderProg::ValueChangedHandle);
	Net = new Network();
	Query = new QueryHandler("ConnQHDatabase");
	Loader->JoinNetwork(Net);
	Loader->JoinQueryHandler(Query);
	Verifier->JoinNetwork(Net);
	Verifier->JoinQueryHandler(Query);
}
