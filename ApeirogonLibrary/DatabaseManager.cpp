#include "pch.h"
#include "DatabaseManager.h"

using namespace std;

DatabaseManager::DatabaseManager(const size_t inThreadPoolSize, const size_t inDatabasePoolSize) : mIsRunning(true), mPoolSize(inDatabasePoolSize), mUsedSize(0), mService(nullptr), mConnections(nullptr), mConnectionInfos(nullptr), mTimeStamp(L"DatabaseManager")
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

	mDatabaseHandler = std::make_shared<DatabaseTaskQueue>();
	if (nullptr == mDatabaseHandler)
	{
		return false;
	}

	InitializeDatabase();
	if (false == StartDatabaseManager())
	{
		return false;
	}

	mDatabaseManagerThread = std::thread(&DatabaseManager::DatabaseLoop, this);
	DatabaseLog(L"[DatabaseManager::Prepare()] Thread started working\n");
	PrintConnectionPoolState();

	return true;
}

void DatabaseManager::Shutdown()
{

	mIsRunning = false;

	if (mDatabaseManagerThread.joinable())
	{
		mDatabaseManagerThread.join();
	}

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

bool DatabaseManager::StartDatabaseManager()
{
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

	return true;
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

DatabaseTaskQueuePtr DatabaseManager::GetDatabaseTaskQueue()
{
	return mDatabaseHandler;
}

void DatabaseManager::DatabaseLoop()
{
	HRESULT hResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (hResult == S_FALSE)
	{
		return;
	}

	const long long		totalProcessTime = 0x64;
	long long			processTime = 0;
	long long			sleepTime = 0;

	static long long	keepConnectionTime = 0;
	const long long		maxConnectionTime = 6000;

	while (mIsRunning)
	{
		mTimeStamp.StartTimeStamp();

		mDatabaseHandler->ProcessAsync();

		processTime = static_cast<long long>(mTimeStamp.GetTimeStamp());

		if (processTime > totalProcessTime)
		{
			keepConnectionTime = processTime;
			//wprintf(L"[ADOAsync::AsyncLoop] Over process time %lld(sec)\n", processTime);
		}
		else
		{
			sleepTime = totalProcessTime - processTime;
			//wprintf(L"[ADOAsync::AsyncLoop] Sleep process time %lld(sec)\n", sleepTime);
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

			keepConnectionTime += (processTime + sleepTime);
		}

		if (keepConnectionTime >= maxConnectionTime)
		{
			KeepConnection();
			keepConnectionTime = 0;
		}
	}

	CoUninitialize();
}

void DatabaseManager::ProcessDatabaseTask(const int64 inServiceTimeStamp)
{

	std::vector<ADOAsyncTaskPtr> completeTask;
	bool isTask = mDatabaseHandler->GetDatabaseTasks(completeTask);
	if (false == isTask)
	{
		return;
	}

	for (ADOAsyncTaskPtr& task : completeTask)
	{
		task->Execute();
	}

}

void DatabaseManager::WorkDispatch()
{

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
