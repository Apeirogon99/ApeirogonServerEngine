#include "pch.h"
#include "Service.h"

Service::Service() : mServiceState(EServiceState::Close), mSessionManager(nullptr), mListener(nullptr), mIOCPServer(nullptr), mThreadManager(nullptr), mLoggerManager(nullptr), mDatabaseManager(nullptr), mDataManager(nullptr), mTaskManager(nullptr), mServiceTime(L"Server")
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

	while (IsServiceOpen())
	{

		const int64 serviceTimeStamp = GetServiceTimeStamp();
		scheudlerTimeStamp.StartTimeStamp();

		while (processTime < serviceTimeStamp + maxProcessTime)
		{
			
			this->mTaskManager->ProcessTask(serviceTimeStamp);

			this->mDatabaseManager->ProcessTask();

			processTime = scheudlerTimeStamp.GetTimeStamp() + serviceTimeStamp;
		}

		mTaskManager->Tick();

		//TODO: 요청만 하게
		mSessionManager->WorkDispatch();

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

	OnServiceOpen();

	ServiceScheudler();

	return true;
}

void Service::ServiceClose()
{
	if (true == IsServiceOpen())
	{
		SetServiceState(EServiceState::Close);

		mThreadManager->Shutdown();

		mDatabaseManager->Shutdown();

		mSessionManager->Shutdown();

		mDataManager->Shutdown();

		mListener->Shutdown();

		mIOCPServer->Shutdown();

		mLoggerManager->Shutdown();

		mThreadManager.reset();
		mDatabaseManager.reset();
		mSessionManager.reset();
		mDataManager.reset();
		mListener.reset();
		mIOCPServer.reset();
		mLoggerManager.reset();
	}

	OnServiceClose();

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

	if (mTaskManager == nullptr || false == mTaskManager->Prepare(shared_from_this()))
	{
		ServiceLog(L"Service::Prepare() : failed to prepare for task manager\n");
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

bool Service::SetTaskManager(TaskManagerPtr& inTaskManager)
{
	mTaskManager = std::move(inTaskManager);
	if (nullptr == mTaskManager)
	{
		ServiceLog(L"Service::SetTaskManager() : Invalid task manager\n");
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

TaskManagerPtr Service::GetTaskManager() const
{
	return mTaskManager;
}

DatabaseManagerPtr Service::GetDatabaseManager() const
{
	return mDatabaseManager;
}

void Service::ServiceLog(const WCHAR* log, ...)
{
	mLoggerManager->WriteLog(log);
}
