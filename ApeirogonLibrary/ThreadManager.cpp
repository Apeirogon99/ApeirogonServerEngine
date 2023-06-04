#include "pch.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager(const uint32 maxThreadCount, const uint32 inMaxThreadTimeOut) : maxThreadCount(maxThreadCount), maxThreadTimeOut(inMaxThreadTimeOut), mThreadProcessTime(L"ThreadProcess")
{
}

ThreadManager::~ThreadManager()
{
	wprintf(L"[ThreadManager::~ThreadManager()]\n");
}

bool ThreadManager::Prepare(ServicePtr service)
{
	this->mService = service;
	if (nullptr == mService)
	{
		return false;
	}

	for (uint32 curCount = 0; curCount < maxThreadCount; ++curCount)
	{
		//thread = std::thread(&Service::DoWork, mService.get());
		mThreads.emplace_back(std::thread(&ThreadManager::DoWorkThreads, this, maxThreadTimeOut));
	}

	ThreadLog(L"[ThreadManager::Prepare()] %ld Threads started working\n", maxThreadCount);

    return true;
}

void ThreadManager::Shutdown()
{
	StopWorkThreads();

	ThreadLog(L"[ThreadManager::Shutdown()] Thread manager successfully shutdown\n");
	mService.reset();
}

void ThreadManager::DoWorkThreads(const uint32 inMaxProcessTime)
{

	const int64 maxProcessTime = inMaxProcessTime;
	int64		processTime = 0;

	while (mService->IsServiceOpen())
	{

		mThreadProcessTime.StartTimeStamp();

		while (processTime <= maxProcessTime)
		{
			mService->GetIOCPServer()->WorkDispatch(static_cast<DWORD>(maxProcessTime - processTime));

			processTime = mThreadProcessTime.GetTimeStamp();
		}

		//mService->GetSessionManager()->WorkDispatch();
	}

}

void ThreadManager::StopWorkThreads()
{
	for (std::thread& thread : mThreads)
	{
		if (thread.joinable())
		{
			//DestroyTLS();
			thread.join();
		}
	}

	ThreadLog(L"[ThreadManager::WorkThreads()] %ld Threads stop working\n", maxThreadCount);
}


void ThreadManager::InitTLS()
{
	std::cout << GetCurrentThreadId() << " : InitTLS" << std::endl;
}

void ThreadManager::DestroyTLS()
{
	OleUninitialize();
}

void ThreadManager::ThreadLog(const WCHAR* log, ...)
{
	mService->GetLoggerManager()->WriteLog(log);
}
