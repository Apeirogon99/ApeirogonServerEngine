#include "pch.h"
#include "DatabaseManager.h"

using namespace std;

DatabaseManager::DatabaseManager(const uint32 inThreadPoolSize, const uint32 inDatabasePoolSize) : mPoolSize(inDatabasePoolSize), mUsedSize(0), mService(nullptr), mConnections(nullptr), mConnectionInfos(nullptr), mAsyncTaskQueue(0x1000), mDatabaseTaskQueue(0x1000), mFastSpinLock()
{
	mConnections = new ADOConnection[inDatabasePoolSize]();
	mConnectionInfos = new ADOConnectionInfo[inDatabasePoolSize]();
}

DatabaseManager::~DatabaseManager()
{
	if (mConnections)
	{
		delete[] mConnections;
	}

	if (mConnectionInfos)
	{
		delete[] mConnectionInfos;
	}

	mConnections = nullptr;
	mConnectionInfos = nullptr;

	wprintf(L"[DatabaseManager::~DatabaseManager()]\n");
}

bool DatabaseManager::Prepare(ServicePtr service)
{
	this->mService = service;
	if (nullptr == mService)
	{
		return false;
	}

	HRESULT hResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (hResult == S_FALSE)
	{
		return false;
	}

	InitializeDatabase();

	if (mUsedSize == 0)
	{
		return false;
	}

	for (size_t index = 0; index < mUsedSize; ++index)
	{
		ADOConnection& connection = mConnections[index];
		ADOConnectionInfo& connectionInfo = mConnectionInfos[index];
		if (!connection.IsOpen())
		{
			connection.Open(connectionInfo);
		}
	}

	for (uint32 index = 0; index < mPoolSize; ++index)
	{
		mThreads.emplace_back(std::thread(&DatabaseManager::DoWorkThreads, this));
	}

	DatabaseLog(L"[DatabaseManager::Prepare()] %ld Threads started working\n", mPoolSize);
	PrintConnectionPoolState();

	return true;
}

void DatabaseManager::Shutdown()
{


	for (size_t index = 0; index < mUsedSize; ++index)
	{
		ADOConnection& connection = mConnections[index];
		if (connection.IsOpen())
		{
			connection.Close();
		}
	}

	CoUninitialize();
	DatabaseLog(L"[DatabaseManager::Shutdown] Database manager successfully shutdown\n");
	mService.reset();
}

void DatabaseManager::PushConnectionPool(ADOConnection& inConnection, const ADOConnectionInfo& inConnectioninfo)
{
	mConnections[mUsedSize] = inConnection;
	mConnectionInfos[mUsedSize] = inConnectioninfo;
	mUsedSize++;
}

void DatabaseManager::PrintConnectionPoolState()
{
	if (mUsedSize == 0)
	{
		return;
	}

	DatabaseLog(L"[Current ConnectionPool States]\n");
	wprintf(L"{\n");
	for (size_t index = 0; index < mUsedSize; ++index)
	{
		ADOConnection& connection = mConnections[index];
		ADOConnectionInfo& connectionInfo = mConnectionInfos[index];

		wprintf(L"\t[Database::%ws] - ", connection.GetDatabaseName());
		if (connection.IsOpen())
		{
			wprintf(L"Open\n");
		}
		else
		{
			wprintf(L"Close\n");
		}
	}
	wprintf(L"}\n");
}

void DatabaseManager::DoWorkThreads()
{
	HRESULT hResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (hResult == S_FALSE)
	{
		return;
	}

	while (mService->IsServiceOpen())
	{
		FastLockGuard lockGuard(mFastSpinLock);
		const ADOAsyncTaskPtr* PeekItem = mAsyncTaskQueue.Peek();
		if (nullptr == PeekItem)
		{
			return;
		}

		if (PeekItem->get()->mADOCommand.IsExecuteComplete())
		{
			ADOAsyncTaskPtr dequeueItem;
			mAsyncTaskQueue.Dequeue(dequeueItem);
			mDatabaseTaskQueue.Enqueue(std::move(dequeueItem));
		}
	}

	CoUninitialize();
}

void DatabaseManager::ProcessTask()
{

	std::vector<ADOAsyncTaskPtr> completeTask;
	{
		FastLockGuard lockGuard(mFastSpinLock);
		if (mDatabaseTaskQueue.IsEmpty())
		{
			return;
		}

		mDatabaseTaskQueue.Dequeue(completeTask);
	}

	for (ADOAsyncTaskPtr& task : completeTask)
	{
		task->Execute();
	}

}

void DatabaseManager::KeepConnection()
{
	if (mUsedSize == 0)
	{
		return;
	}

	for (size_t index = 0; index < mUsedSize; ++index)
	{
		ADOConnection& connection = mConnections[index];
		ADOConnectionInfo& connectionInfo = mConnectionInfos[index];

		if (!connection.IsOpen() || connection->GetState() != adStateConnecting)
		{
			connection.OpenEx(connectionInfo);
		}
	}

}

bool DatabaseManager::PushAsyncTaskQueue(PacketSessionPtr& inSession, ADOConnection& inADOConnection, ADOCommand& inADOCommand, ADORecordset& inADORecordset, ADOCallBack& inADOCallBack)
{
	FastLockGuard lockGuard(mFastSpinLock);
	ADOAsyncTaskPtr newItem = std::make_shared<ADOAsyncTask>(inSession, inADOConnection, inADOCommand, inADORecordset, inADOCallBack);
	const bool result = mAsyncTaskQueue.Enqueue(std::move(newItem));
	return result;
}

void DatabaseManager::DatabaseLog(const WCHAR* log, ...)
{
	if (!mService)
	{
		return;
	}

	LoggerManagerPtr logger = mService->GetLoggerManager();
	if (!logger)
	{
		return;
	}

	logger.get()->WriteLog(log);
}
