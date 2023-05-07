#include "pch.h"
#include "DatabaseManager.h"

using namespace std;

DatabaseManager::DatabaseManager(const size_t poolSize) : mIsRunning(true), mPoolSize(poolSize), mUsedSize(0), mService(nullptr), mConnections(nullptr), mConnectionInfos(nullptr), mTimeStamp(L"DatabaseManager")
{
	mConnections = new ADOConnection[mPoolSize]();
	mConnectionInfos = new ADOConnectionInfo[mPoolSize]();
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

bool DatabaseManager::Prepare(const ServicePtr& service)
{
	mService = service;
	if (nullptr == mService)
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

void DatabaseManager::DatabaseLoop()
{
	// 프로세스를 실행시키는 시간이 짧을 수 있으니까 일부로 sleep을 사용해 잠시 쉬게 해준다
	// 기준은 totalProcessTime과 비교하여 만약에 넘는다면 문제가 있는 것이고
	// 안넘는다면 수행한 processTime에서 빼서 쉬게 한다

	// 만약에 큐에 작업이 존재하지 않는다면 쉬게한다

	const long long		totalProcessTime = 0x3E8;
	long long			processTime = 0;
	long long			sleepTime = 0;

	static long long	keepConnectionTime = 0;
	const long long		maxConnectionTime = 6000;

	while (mIsRunning)
	{
		mTimeStamp.StartTimeStamp();
		mADOTask.ProcessAsync();
		processTime = static_cast<long long>(mTimeStamp.GetTimeStamp());

		if (processTime > totalProcessTime)
		{
			keepConnectionTime = processTime;
			wprintf(L"[ADOAsync::AsyncLoop] Over process time %lld(sec)\n", processTime);
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
}

void DatabaseManager::ProcessDatabaseTask()
{
	ADOTask& databaseTask = GetTask();
	if (databaseTask.IsCompletionWork())
	{
		ADOItem item;
		if (true == databaseTask.GetCompeltionWork(item))
		{
			PacketSessionPtr& session	= item.mSession;
			ADOCommand& command			= item.mADOCommand;
			ADORecordset& recordset		= item.mADORecordset;
			ADOCallBack& callback		= item.mADOCallBack;

			callback(session, command, recordset);
		}
	}
}

ADOTask& DatabaseManager::GetTask()
{
	return mADOTask;
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
	mService->GetLoggerManager()->WriteLog(log);
}
