#include "network.h"

Network::Network()
{
	List = curl_slist_append(List, "method: GET");
	List = curl_slist_append(List, "Accept: application/json");
	List = curl_slist_append(List, "HH-User-Agent: СЮДА_НАЗВАНИЕ_ПРИЛОЖЕНИЯ (СЮДА_ВАШУ_ПОЧТУ)");
	List = curl_slist_append(List, "Authorization: Bearer СЮДА_ВАШ_ТОКЕН");
	LogProcs = std::thread::hardware_concurrency();
	SemFreeIds = new std::counting_semaphore<>{ LogProcs };
	CurlSlots = new CURLInfo[LogProcs];
	EvtWaitTickets = new HANDLE[LogProcs];
	MultiCurl = curl_multi_init();
	for (char i = 0; i < LogProcs; ++i)
	{
		EvtWaitTickets[i] = CreateEventA(NULL, FALSE, FALSE, NULL);
		PutFreeId(i);
	}
	MDManager = new std::thread(&Network::MultiDownload, this);
}

Network::~Network()
{
	if (MDManager)
	{
		MDManager->detach();
		delete MDManager;
		MDManager = nullptr;
	}
	if (SemFreeIds)
	{
		delete SemFreeIds;
		SemFreeIds = nullptr;
	}
	if (CurlSlots)
	{
		for (int i = 0; i < LogProcs; ++i)
		{
			curl_easy_cleanup(CurlSlots[i].curl);
		}
		delete[] CurlSlots;
		CurlSlots = nullptr;
	}
	for (int i = 0; i < LogProcs; ++i)
	{
		if (EvtWaitTickets)
		{
			CloseHandle(EvtWaitTickets[i]);
			EvtWaitTickets = nullptr;
		}
	}
	if (MultiCurl)
	{
		curl_multi_cleanup(MultiCurl);
	}
}

void Network::Stop()
{
	Active = false;
	SemPreparedIds.release(9999);
	for(int i = 0; i < LogProcs; ++i)
	{
		SetEvent(EvtWaitTickets[i]);
	}
}

void Network::MultiDownload()
{
	CURLMsg* MsgCode = nullptr;
	int Alive = 0, MsgAlive = 0;
	while (Active)
	{
		SemPreparedIds.acquire();
		if(!Active)
		{
			return;
		}
		SemPreparedIds.release();
		while (!IsEmptyPreparedId())
		{
			curl_multi_add_handle(MultiCurl, CurlSlots[GetPreparedId()].curl);
		}
		curl_multi_perform(MultiCurl, &Alive);
		while ((MsgCode = curl_multi_info_read(MultiCurl, &MsgAlive)))
		{
			if (MsgCode->msg = CURLMSG_DONE)
			{
				for (int i = 0; i < LogProcs; ++i)
				{
					if (CurlSlots[i].curl == MsgCode->easy_handle)
					{
						curl_multi_remove_handle(MultiCurl, MsgCode->easy_handle);
						SemPreparedIds.acquire();
						SetEvent(EvtWaitTickets[i]);
						break;
					}
				}
			}

		}
		curl_multi_wait(MultiCurl, NULL, 0, 1000, NULL);
	}
}

void Network::Get(const std::string& Url, std::string& Data)
{
	char Id = GetFreeId();
	if(Active)
	{
		CurlSlots[Id].PrepareCurl(Url, List, Data);
		PutPreparedId(Id);
		WaitForSingleObject(EvtWaitTickets[Id], 10000);
		PutFreeId(Id);
		SemFreeIds->release();
	}
}

void Network::PutFreeId(const char& Id)
{
	MtxFreeIds.lock();
	FreeIds.push(Id);
	MtxFreeIds.unlock();
}

char Network::GetFreeId()
{
	char Id = 0;
	SemFreeIds->acquire();
	MtxFreeIds.lock();
	Id = FreeIds.front();
	FreeIds.pop();
	MtxFreeIds.unlock();
	return Id;
}

void Network::PutPreparedId(const char& Id)
{
	MtxPreparedIds.lock();
	PreparedIds.push(Id);
	SemPreparedIds.release();
	MtxPreparedIds.unlock();
}

char Network::GetPreparedId()
{
	char Id = 0;
	MtxPreparedIds.lock();
	Id = PreparedIds.front();
	PreparedIds.pop();
	MtxPreparedIds.unlock();
	return Id;
}

bool Network::IsEmptyPreparedId()
{
	bool IsEmpty = false;
	MtxPreparedIds.lock();
	IsEmpty = PreparedIds.empty();
	MtxPreparedIds.unlock();
	return IsEmpty;
}

CURLInfo::CURLInfo()
{
	curl = curl_easy_init();
}

void CURLInfo::PrepareCurl(const std::string& Url, const curl_slist* List, std::string& Data)
{
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 2.0; Windows NT 5.0; Trident/4.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0)");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, List);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &Data);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CURLInfo::write_callback);
	curl_easy_setopt(curl, CURLOPT_URL, Url.c_str());
}

size_t CURLInfo::write_callback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
	size_t ActualSize = size * nmemb;
	((std::string*)userdata)->append((char*)ptr, ActualSize);
	return ActualSize;
}
