#include "pch.h"
#include "Service.h"

Service::Service() : mServiceState(EServiceState::Close), mSessionManager(nullptr), mIOCPServer(nullptr), mListener(nullptr), mServiceTime(L"Server")
{
	setlocale(LC_ALL, "");
}

Service::~Service()
{
	wprintf(L"[Service::~Service()] Close service (Running time : %lld)\n", mServiceTime.GetTimeStamp());
}

int64 Service::GetServiceTimeStamp()
{
	return mServiceTime.GetTimeStamp();
}

void Service::ServiceScheudler()
{
	TimeStamp	scheudlerTimeStamp(L"Scheudler");
	const int64 maxProcessTime = 33;
	int64		processTime = 0;
	int64		sleepTime = 0;

	while (IsServiceOpen())
	{
		scheudlerTimeStamp.StartTimeStamp();
		const int64 serviceTimeStamp = mServiceTime.GetTimeStamp();

		this->mSessionManager->ProcessNetworkTask(serviceTimeStamp);
		
		this->mDatabaseManager->ProcessDatabaseTask(serviceTimeStamp);

		this->mSessionManager->ProcessSnapShot();

		processTime = scheudlerTimeStamp.GetTimeStamp();
		if (processTime > maxProcessTime)
		{
			wprintf(L"[Service::ServiceScheudler] Over process time %lld(sec)\n", processTime);
		}
		else
		{
			sleepTime = maxProcessTime - processTime;
			//wprintf(L"[Service::ServiceScheudler] Sleep process time %lld(sec)\n", sleepTime);
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
		}
	}
}

bool Service::ServiceOpen()
{
	if (false == SettingService())
	{
		ServiceLog(L"Service::ServiceOpen() : failed to setting service\n");
		return false;
	}

	SetServiceState(EServiceState::Open);

	if (false == Prepare())
	{
		ServiceLog(L"Service::ServiceOpen() : failed to prepare\n");
		return false;
	}

	std::wstring ip;
	if (false == mListener->GetIpAddress()->GetIp(ip))
	{
		return false;
	}
	const uint16 port = mListener->GetIpAddress()->GetPort();
	ServiceLog(L"[Service::ServiceOpen()] IP[%ws] Port[%d] service is Open\n", ip.c_str(), port);

	mServiceTime.StartTimeStamp();

	ServiceScheudler();

	return true;
}

void Service::ServiceClose()
{
	if (true == IsServiceOpen())
	{
		SetServiceState(EServiceState::Close);

		mSessionManager->Shutdown();

		mDataManager->Shutdown();

		mDatabaseManager->Shutdown();

		mListener->Shutdown();

		mThreadManager->Shutdown();

		mIOCPServer->Shutdown();

		mLoggerManager->Shutdown();

		mSessionManager.reset();
		mDataManager.reset();
		mDatabaseManager.reset();
		mListener.reset();
		mThreadManager.reset();
		mIOCPServer.reset();
		mLoggerManager.reset();
	}

}

bool Service::Prepare()
{
	
	ServicePtr servicePtr = shared_from_this();

	if (mLoggerManager == nullptr || false == mLoggerManager->Prepare(shared_from_this()))
	{
		ServiceLog(L"Service::SetLoggerManager() : failed to prepare for logger manager\n");
		return false;
	}

	if (mDataManager == nullptr || false == mDataManager->Prepare(shared_from_this()))
	{
		ServiceLog(L"Service::Prepare() : failed to prepare for data manager\n");
		return false;
	}

	if (mDatabaseManager == nullptr || false == mDatabaseManager->Prepare(shared_from_this()))
	{
		ServiceLog(L"Service::Prepare() : failed to prepare for database manager\n");
		return false;
	}

	if (mIOCPServer == nullptr || false == mIOCPServer->Prepare(shared_from_this()))
	{
		ServiceLog(L"Service::Prepare() : failed to prepare for iocp Server\n");
		return false;
	}

	if (mSessionManager == nullptr || false == mSessionManager->Prepare(shared_from_this()))
	{
		ServiceLog(L"Service::Prepare() : failed to prepare for session manager\n");
		return false;
	}

	if (mListener == nullptr || false == mListener->Prepare(shared_from_this()))
	{
		ServiceLog(L"Service::Prepare() : failed to prepare for listener\n");
		return false;
	}

	if (mThreadManager == nullptr || false == mThreadManager->Prepare(shared_from_this()))
	{
		ServiceLog(L"Service::Prepare() : failed to prepare for thread manager\n");
		return false;
	}
	
	return true;
}

bool Service::SetIOCPServer(IOCPServerPtr& iocpServer)
{
	mIOCPServer = std::move(iocpServer);
	if (nullptr == mIOCPServer)
	{
		ServiceLog(L"Service::SetIOCPServer() : Invalid IOCPServer\n");
		return false;
	}

	return true;
}

bool Service::SetListener(ListenerPtr& listener)
{
	mListener = std::move(listener);
	if (nullptr == mListener)
	{
		ServiceLog(L"Service::SetSessionListener() : Invalid listener\n");
		return false;
	}

	return true;
}

bool Service::SetSessionManager(SessionManagerPtr& sessionManager)
{
	mSessionManager = std::move(sessionManager);
	if (nullptr == mSessionManager)
	{
		ServiceLog(L"Service::SetSessionManager() : Invalid session manager\n");
		return false;
	}

	return true;
}

bool Service::SetThreadManager(ThreadManagerPtr& threadManager)
{
	mThreadManager = std::move(threadManager);
	if (nullptr == mThreadManager)
	{
		ServiceLog(L"Service::SetThreadManager() : Invalid thread manager\n");
		return false;
	}

	return true;
}

bool Service::SetLoggerManager(LoggerManagerPtr& loggerManager)
{
	mLoggerManager = std::move(loggerManager);
	if (nullptr == mLoggerManager)
	{
		ServiceLog(L"Service::SetLoggerManager() : Invalid logger manager\n");
		return false;
	}

	return true;
}

bool Service::SetDatabaseManager(DatabaseManagerPtr& inDatabase)
{
	mDatabaseManager = std::move(inDatabase);
	if (nullptr == mDatabaseManager)
	{
		ServiceLog(L"Service::SetDatabaseManager() : Invalid database manager\n");
		return false;
	}

	return true;
}

bool Service::SetDataManager(DataManagerPtr& inDataManager)
{
	mDataManager = std::move(inDataManager);
	if (nullptr == mDataManager)
	{
		ServiceLog(L"Service::SetDatabaseManager() : Invalid data manager\n");
		return false;
	}

	return true;
}

void Service::SetServiceState(const EServiceState state)
{
	mServiceState = state;
}

bool Service::IsServiceOpen() const
{
	return mServiceState == EServiceState::Open ? true : false;
}

EServiceState Service::GetState() const
{
	return mServiceState;
}

ServicePtr Service::GetPtr() const
{
	return nullptr;
}

SessionManagerPtr Service::GetSessionManager() const
{
	return mSessionManager;
}

ListenerPtr Service::GetListener() const
{
	return mListener;
}

IOCPServerPtr Service::GetIOCPServer() const
{
	return mIOCPServer;
}

ThreadManagerPtr Service::GetThreadManager() const
{
	return mThreadManager;
}

LoggerManagerPtr Service::GetLoggerManager() const
{
	return mLoggerManager;
}

DataManagerPtr Service::GetDataManager() const
{
	return mDataManager;
}

DatabaseManagerPtr Service::GetDatabaseManager() const
{
	return mDatabaseManager;
}

void Service::ServiceLog(const WCHAR* log, ...)
{
	mLoggerManager->WriteLog(log);
}
