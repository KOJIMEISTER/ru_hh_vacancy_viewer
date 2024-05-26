#include "backend.h"

Backend::Backend(QObject *parent)
	: QObject{parent}
{
	Net = new RemoteDatabase("RemoteDBBackend", this);
	ViewedVacIds.resize(134217728);
}

Backend::~Backend()
{
	delete Net;
}

void Backend::makeVacToPredictRequest(int Index, int VacId, int RoleId)
{
	QString Description = Net->GetVacancyDescrition(VacId);
	emit Classifier->RequestPredict(Index, Description, RoleId);
}

void Backend::JoinClassifier(Classificator* Classifier)
{
	this->Classifier = Classifier;
	connect(Classifier, &Classificator::sendPredict, this, &Backend::sendVacPredict);
}

int Backend::getViewedVacsCount()
{
	int Count = 0;
	if(DB != nullptr)
	{
		QSqlQuery Query = DB->select(QString("select count(*) from vacancies where by_profile='%1'").arg(getCurrentProfile()));
		if(Query.next())
		{
			Count = Query.value(0).toInt();
		}
	}
	return Count;
}

void Backend::addProfile(QString Name)
{
	if(DB != nullptr)
	{
		if(DB->insert(QString("insert into profiles (profile_name) values ('%1')").arg(Name)))
		{
			setCurrentProfile(Name);
		}

	}
}

void Backend::deleteProfile(QString Name)
{
	if(DB != nullptr)
	{
		if(Name == getCurrentProfile())
		{
			setCurrentProfile("Базовый");
		}
		DB->insert(QString("delete from vacancies where by_profile ='%1'").arg(Name));
		DB->insert(QString("delete from histories where by_profile ='%1'").arg(Name));
		DB->insert(QString("delete from profiles where profile_name ='%1'").arg(Name));
	}
}

QString Backend::getCurrentProfile()
{
	QString Name = "";
	if(DB != nullptr)
	{
		QSqlQuery Query = DB->select("select property_value from app_properties where property_name = 'current_profile'");
		if(Query.next())
		{
			Name = Query.value(0).toString();
		}
		Query.clear();
	}
	return Name;
}

void Backend::setCurrentProfile(QString Name)
{
	if(DB != nullptr)
	{
		DB->insert(QString("update app_properties set property_value='%1' where property_name='current_profile'").arg(Name));
		QSqlQuery Query = DB->select(QString("select vac_id from vacancies where by_profile='%1'").arg(getCurrentProfile()));
		ViewedVacIds.clear();
		ViewedVacIds.resize(134217728);
		while(Query.next())
		{
			int Id = Query.value(0).toInt();
			ViewedVacIds.setBit(Id, true);
		}
		Query.clear();
	}
}

QVariantList Backend::getAllProfiles()
{
	QVariantList ProfilesList;
	if(DB != nullptr)
	{
		QString CurrentProfile = getCurrentProfile();
		QSqlQuery Query = DB->select("select profile_name from profiles");
		if(Query.lastError().isValid())
		{
			qDebug() << Query.lastError();
		}
		else
		{
			while(Query.next())
			{
				QVariantMap ProfilesMap;
				ProfilesMap.insert("name", Query.value(0).toString());
				ProfilesMap.insert("current", (Query.value(0).toString() == CurrentProfile) ? true : false);
				ProfilesList.append(ProfilesMap);
			}
		}
		Query.clear();
	}
	return ProfilesList;
}

QVariantMap Backend::jsonParamsToVarList(QString Params)
{
	QVariantMap ParamsMap;
	QJsonParseError Error;
	QJsonDocument JsonParams = QJsonDocument::fromJson(Params.toUtf8(), &Error);
	if(Error.error == QJsonParseError::NoError)
	{
		ParamsMap.insert("shift", JsonParams["shift"].toBool());
		ParamsMap.insert("remote", JsonParams["remote"].toBool());
		ParamsMap.insert("agile", JsonParams["agile"].toBool());
		ParamsMap.insert("fullday", JsonParams["fullday"].toBool());
		ParamsMap.insert("parttime", JsonParams["parttime"].toBool());
		ParamsMap.insert("from6", JsonParams["from6"].toBool());
		ParamsMap.insert("from3to6", JsonParams["from3to6"].toBool());
		ParamsMap.insert("from1to3", JsonParams["from1to3"].toBool());
		ParamsMap.insert("unexp", JsonParams["unexp"].toBool());
		ParamsMap.insert("intern", JsonParams["intern"].toBool());
		ParamsMap.insert("fulltime", JsonParams["fulltime"].toBool());
		QVariantList TagsList;
		for(const auto Tag : JsonParams["tags"].toArray())
		{
			QVariantMap TagMap;
			TagMap.insert("name", Tag.toObject().value("name").toString());
			TagMap.insert("type", Tag.toObject().value("type").toBool());
			TagsList.append(TagMap);
		}
		ParamsMap.insert("tags", TagsList);
		QVariantList CitiesList;
		for(const auto City : JsonParams["cities"].toArray())
		{
			QVariantMap CityMap;
			CityMap.insert("name", City.toObject().value("name").toString());
			CitiesList.append(CityMap);
		}
		ParamsMap.insert("cities", CitiesList);
		QVariantList RolesList;
		for(const auto Role : JsonParams["roles"].toArray())
		{
			QVariantMap RoleMap;
			RoleMap.insert("name", Role.toObject().value("name").toString());
			RolesList.append(RoleMap);
		}
		ParamsMap.insert("roles", RolesList);
	}
	else
	{
		qDebug() << Error.errorString();
	}
	return ParamsMap;
}

void Backend::renameHistory(QString Name, int HistId)
{
	if(DB != nullptr)
	{
		DB->insert(QString("update histories set history_name='%1' where history_id=%2").arg(Name).arg(HistId));
	}
}

void Backend::updateHistoryViewedCount(int HistId)
{
	if(DB != nullptr)
	{
		qsizetype NotWatched = 0, AllFound = 0, Watched = 0;
		QSqlQuery Query = DB->select(QString("select history_params from histories where history_id='%1'").arg(HistId));
		if(Query.next())
		{
			NotWatched = getVacToPrepareBySearch(Query.value(0).toString(), false, true).count();
			AllFound = getVacToPrepareBySearch(Query.value(0).toString(), true, true).count();
			Watched = AllFound - NotWatched;
		}
		Query.clear();
		DB->insert(QString("update histories set history_found='%1', history_watched='%2' where history_id='%3'").arg(AllFound).arg(Watched).arg(HistId));
	}
}

void Backend::deleteHistory(int HistId)
{
	if(DB != nullptr)
	{
		DB->insert("delete from histories where history_id = " + QString::number(HistId));
	}
}

QVariantList Backend::getSearchHistories()
{
	QVariantList HistoriesList;
	if(DB != nullptr)
	{
		QSqlQuery Query = DB->select(QString("select history_id, history_name, history_params, history_found, history_watched from histories where by_profile='%1'").arg(getCurrentProfile()));
		while(Query.next())
		{
			QVariantMap HistoryMap;
			HistoryMap.insert("histId", Query.value(0).toInt());
			HistoryMap.insert("name", Query.value(1).toString());
			HistoryMap.insert("params", Query.value(2).toString());
			HistoryMap.insert("found", Query.value(3).toInt());
			HistoryMap.insert("watched", Query.value(4).toInt());
			HistoriesList.append(HistoryMap);
		}
		Query.clear();
	}
	return HistoriesList;
}

void Backend::clearAllViews()
{
	if(DB != nullptr)
	{
		DB->insert(QString("delete from vacancies where by_profile='%1'").arg(getCurrentProfile()));
		ViewedVacIds.clear();
		ViewedVacIds.resize(134217728);
	}
}

bool Backend::VacIsViewed(int VacId)
{
	return ViewedVacIds.at(VacId);
}

void Backend::updateVacViewed(QVariantMap VacPreiview, int)
{
	if(DB != nullptr and !VacIsViewed(VacPreiview.value("vacId").toInt()))
	{
		QString StringQuery = "insert into vacancies (vac_id, vac_viewed_at, by_profile) values (";
		StringQuery += QString::number(VacPreiview.value("vacId").toInt());
		StringQuery += ",";
		StringQuery += QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());
		StringQuery += ",'";
		StringQuery += getCurrentProfile();
		StringQuery += "')";
		if(DB->insert(StringQuery))
		{
			ViewedVacIds.setBit(VacPreiview.value("vacId").toInt(), true);
		}
	}
}

void Backend::JoinDatabase(Database* DB)
{
	this->DB = DB;
	QSqlQuery Query = DB->select(QString("select vac_id from vacancies where by_profile='%1'").arg(getCurrentProfile()));
	while(Query.next())
	{
		int Id = Query.value(0).toInt();
		ViewedVacIds.setBit(Id, true);
	}
	Query.clear();
}

void Backend::JoinWoker(BackendWorker* Worker)
{
	this->Worker = Worker;
	connect(this, SIGNAL(makeVacPreviewInfoRequest(int, int)), this, SLOT(ProxyRequest(int, int)));
	connect(Worker, SIGNAL(sendVacPreview(QVariantMap, int)), this, SIGNAL(sendVacPreview(QVariantMap, int)));
	connect(Worker, SIGNAL(sendVacPreview(QVariantMap, int)), this, SLOT(updateVacViewed(QVariantMap, int)));
}

void Backend::ProxyRequest(int VacId, int Index)
{
	if(Worker != nullptr)
	{
		Worker->makeVacPreviewInfoRequest(VacId, Index);
	}
}

void Backend::PutSearch(QString Params)
{
	if(DB != nullptr)
	{
		QByteArray Hashed = QCryptographicHash::hash(Params.toLocal8Bit(), QCryptographicHash::Md5).toHex();
		QSqlQuery Query = DB->select(QString("select * from histories where history_hash='%1' and by_profile='%2'").arg(QString::fromLocal8Bit(Hashed)).arg(getCurrentProfile()));
		if(!Query.next())
		{
			QString StringQuery = "insert into histories (history_name, history_params, history_hash, by_profile) values ('%1','%2','%3','%4')";
			StringQuery = StringQuery.arg(QDateTime::currentDateTime().toString("dd.MM.yy HH:mm:ss")).arg(Params).arg(QString::fromLocal8Bit(Hashed)).arg(getCurrentProfile());
			DB->insert(StringQuery);
		}
		Query.clear();
	}
}

QVariantList Backend::getVacToPrepareBySearch(QString Params, bool LoadViewed, bool Order)
{
	PutSearch(Params);
	QVariantList VacPrepareInfoList;
	QSqlQuery VacIds = Net->GetVacancySearch(Params, Order);
	while(VacIds.next())
	{
		QVariantMap VacContent;
		QVariantMap VacInfo;
		int VacId = VacIds.value("vac_id").toInt();
		bool IsViewed = VacIsViewed(VacId);
		if(IsViewed)
		{
			if(LoadViewed)
			{
				VacContent.insert("vacId", VacId);
				VacInfo.insert("content", VacContent);
				VacInfo.insert("loaded", false);
				VacInfo.insert("viewed", true);
				VacInfo.insert("predict", "");
				VacPrepareInfoList.push_back(VacInfo);
			}
		}
		else
		{
			VacContent.insert("vacId", VacId);
			VacInfo.insert("content", VacContent);
			VacInfo.insert("loaded", false);
			VacInfo.insert("viewed", false);
			VacInfo.insert("predict", "");
			VacPrepareInfoList.push_back(VacInfo);
		}
	}
	VacIds.clear();
	return VacPrepareInfoList;
}

QVariantList Backend::getSimilarTags(QString Tag)
{
	QVariantList TagsList;
	QList<QPair<QString, int>> Tags = Net->GetSimillarTags(Tag);
	for(const auto& Tag : Tags)
	{
		QVariantMap TagMap;
		TagMap.insert("name", Tag.first);
		TagMap.insert("count", Tag.second);
		TagsList.push_back(TagMap);
	}
	return TagsList;
}

QVariantList Backend::getSimilarCities(QString City)
{
	QVariantList CitiesList;
	QList<QPair<QString, int>> Cities = Net->GetSimillarCities(City);
	for(const auto& City : Cities)
	{
		QVariantMap CityMap;
		CityMap.insert("name", City.first);
		CityMap.insert("count", City.second);
		CitiesList.push_back(CityMap);
	}
	return CitiesList;
}

QVariantList Backend::getAllRoles()
{
	QVariantList RolesList;
	QList<QPair<QString, int>> Roles= Net->GetAllRoles();
	for(const auto& Role : Roles)
	{
		QVariantMap SpecMap;
		SpecMap.insert("name", Role.first);
		SpecMap.insert("count", Role.second);
		RolesList.push_back(SpecMap);
	}
	return RolesList;
}

QString Backend::getVacancyDescription(int VacId)
{
	QString Description = Net->GetVacancyDescrition(VacId);
	return ReformateText(Description);
}

QString Backend::ReformateText(QString TextToFormate)
{
	QString FormatedText;
	QRegularExpression Requirments("(<\\w+>)*[а-я\\w\\- ,.«»:]*(требования|(наши пожелания)|обладают|обязанности|((вам|тебе|чем|что|которые) предстоит)|ожидаем|ждем|требуется|(наши ожидания)|(нам важно)|(ты будешь)|(вы будете)|(нужно (будет)? делать)|(задачи (ждут вас)|(вас ждут))|(зона ответственности)|(необходимые компетенции))[а-я\\w\\- ,.«»:]*", QRegularExpression::PatternOption::CaseInsensitiveOption);
	QRegularExpression Conditions("(<\\w+>)*[а-я\\w\\- ,.«»:]*(предлагаем|предложить|предлагает|условия)[а-я\\w\\- ,.«»:]*", QRegularExpression::PatternOption::CaseInsensitiveOption);
	QRegularExpressionMatch ReqMatch = Requirments.match(TextToFormate);
	QRegularExpressionMatch CondMatch = Conditions.match(TextToFormate);
	qsizetype ReqPos = -1, CondPos = -1;
	if(ReqMatch.hasMatch())
	{
		ReqPos = ReqMatch.capturedStart();
	}
	if(CondMatch.hasMatch())
	{
		CondPos = CondMatch.capturedStart();
	}
	if(ReqPos != -1)
	{
		if(ReqPos <= CondPos)
		{
			QString Important = TextToFormate.mid(ReqPos, CondPos-ReqPos);
			TextToFormate = TextToFormate.remove(ReqPos, CondPos-ReqPos);
			FormatedText.append(Important);
			FormatedText.append(TextToFormate);
			return FormatedText;
		}
		else
		{
			QString Important = TextToFormate.mid(ReqPos, TextToFormate.length()-ReqPos);
			TextToFormate = TextToFormate.remove(ReqPos, TextToFormate.length()-ReqPos);
			FormatedText.append(Important);
			FormatedText.append(TextToFormate);
			return FormatedText;
		}
	}
	else
	{
		return TextToFormate;
	}
}

QVariantList Backend::getVacancyTags(int VacId)
{
	QVariantList TagsList;
	QList<QString> Tags = Net->GetVacancyTags(VacId);
	for(const auto& Tag : Tags)
	{
		QVariantMap TagMap;
		TagMap.insert("name", Tag);
		TagsList.push_back(TagMap);
	}
	return TagsList;
}
