#pragma once
#define CURL_STATICLIB
#include <QDebug>
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <semaphore>
#include <curl/curl.h>

struct CURLInfo
{
	CURLInfo();
	void PrepareCurl(const std::string& Url, const struct curl_slist* List, std::string& Data);
	static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata);
	CURL* curl = nullptr;
};

class Network
{
	void MultiDownload();
	void PutFreeId(const char& Id);
	char GetFreeId();
	void PutPreparedId(const char& Id);
	char GetPreparedId();
	bool IsEmptyPreparedId();
	bool Active = true;

	struct curl_slist* List = NULL;
	std::thread* MDManager = nullptr;

	CURLM* MultiCurl = nullptr;
	std::counting_semaphore<> SemPreparedIds{ 0 };
	std::counting_semaphore<>* SemFreeIds = nullptr;
	HANDLE* EvtWaitTickets = nullptr;
	std::queue<char> PreparedIds;
	std::mutex MtxPreparedIds;

	CURLInfo* CurlSlots = nullptr;
	std::queue<char> FreeIds;
	std::mutex MtxFreeIds;
	unsigned int LogProcs = 1;
public:
	Network();
	~Network();
	void Get(const std::string& Url, std::string& Data);
	void Stop();
};

