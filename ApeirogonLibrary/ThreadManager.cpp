#include "pch.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager(const uint32 maxThreadCount) : maxThreadCount(maxThreadCount)
{
}

ThreadManager::~ThreadManager()
{
	wprintf(L"ThreadManager::~ThreadManager()\n");
}

bool ThreadManager::Prepare(const ServicePtr& service)
{
	mService = service;
	if (nullptr == mService)
	{
		return false;
	}

	for (uint32 curCount = 0; curCount < maxThreadCount; ++curCount)
	{
		mThreads.emplace_back(std::thread());
	}

    return true;
}

void ThreadManager::Shutdown()
{
	JoinThreads();

	mService.reset();

	wprintf(L"LoggerManager::Shutdown() : Thread manager successfully shutdown\n");
}

void ThreadManager::WorkThreads()
{

	for (std::thread& thread : mThreads)
	{
		thread = std::thread(&Service::DoWork, mService.get());
	}

	ThreadLog(L"ThreadManager::WorkThreads() : Threads started working successfully\n");
}

void ThreadManager::JoinThreads()
{
	for (std::thread& thread : mThreads)
	{
		if (thread.joinable())
		{
			DestroyTLS();
			thread.join();
		}
	}
	mThreads.clear();

	ThreadLog(L"ThreadManager::WorkThreads() : Threads closed working successfully\n");
}


void ThreadManager::InitTLS()
{
	std::cout << GetCurrentThreadId() << " : InitTLS" << std::endl;
	HRESULT hResult;
	hResult = OleInitialize(NULL);
	if (FAILED(hResult))
	{
		return;
	}
}

void ThreadManager::DestroyTLS()
{
	OleUninitialize();
}

void ThreadManager::ThreadLog(const WCHAR* log, ...)
{
	mService->GetLoggerManager()->WriteLog(log);
}
