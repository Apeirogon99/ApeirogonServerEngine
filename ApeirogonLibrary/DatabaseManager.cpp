#include "pch.h"
#include "DatabaseManager.h"

using namespace std;

DatabaseManager::DatabaseManager(const size_t poolSize) : mPoolSize(poolSize), mUsedSize(0), mService(nullptr), mConnections(nullptr), mConnectionInfos(nullptr)
{
	mConnections = new ADOConnection[mPoolSize]();
	mConnectionInfos = new ADOConnectionInfo[mPoolSize]();

	mDatabaseManagerThread = std::thread(&DatabaseManager::DatabaseLoop, this);
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

	std::wcout << L"[DatabaseManager::Prepare] Start DatabaseLoop (thread_id:" << mDatabaseManagerThread.get_id() << L")" << std::endl;
	PrintConnectionPoolState();

	return true;
}

void DatabaseManager::Shutdown()
{
	if (mDatabaseManagerThread.joinable())
	{
		std::wcout << L"[DatabaseManager::Shutdown] Stop DatabaseLoop (thread_id:" << mDatabaseManagerThread.get_id() << L")" << std::endl;
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

	wprintf(L"\nPrintConnectionPoolState\n");
	for (size_t index = 0; index < mUsedSize; ++index)
	{
		ADOConnection& connection = mConnections[index];
		ADOConnectionInfo& connectionInfo = mConnectionInfos[index];

		wprintf(L"[Database::%ws] - ", connection.GetDatabaseName());
		if (connection.IsOpen())
		{
			wprintf(L"open\n");
		}
		else
		{
			wprintf(L"close\n");
		}
	}

	wprintf(L"\n");
}

void DatabaseManager::DatabaseLoop()
{
	// ���μ����� �����Ű�� �ð��� ª�� �� �����ϱ� �Ϻη� sleep�� ����� ��� ���� ���ش�
	// ������ totalProcessTime�� ���Ͽ� ���࿡ �Ѵ´ٸ� ������ �ִ� ���̰�
	// �ȳѴ´ٸ� ������ processTime���� ���� ���� �Ѵ�

	// ���࿡ ť�� �۾��� �������� �ʴ´ٸ� �����Ѵ�

	Runtime				time(L"ADOAsync", ETime::ms);
	const long long		totalProcessTime = 0x3E8;
	long long			processTime = 0;
	long long			sleepTime = 0;

	static long long	keepConnectionTime = 0;
	const long long		maxConnectionTime = 6000;

	while (mService->IsServiceOpen())
	{
		time.Start();
		mADOAsync.ProcessAsync();
		processTime = static_cast<long long>(time.End());

		if (processTime > totalProcessTime)
		{
			keepConnectionTime = processTime;
			wprintf(L"[ADOAsync::AsyncLoop] Over process time %lld(sec)\n", processTime);
		}
		else
		{
			sleepTime = totalProcessTime - processTime;
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

ADOAsync& DatabaseManager::GetAsync()
{
	return mADOAsync;
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