#include "pch.h"
#include "Service.h"

Service::Service() : mServiceState(EServiceState::Close), mSessionManager(nullptr), mIOCPServer(nullptr), mListener(nullptr)
{
	WinDump::Init();
}

Service::~Service()
{
	wprintf(L"Service::~Service() : Close service\n");
}

void Service::DoWork()
{
	while (IsServiceOpen())
	{
		this->GetIOCPServer()->WorkDispatch(10);
		//Tick();
	}
}

bool Service::ServiceOpen()
{
	if (false == SettingService())
	{
		ServiceLog(L"Service::ServiceOpen() : failed to setting service\n");
		return false;
	}

	if (false == Prepare())
	{
		ServiceLog(L"Service::ServiceOpen() : failed to prepare\n");
		return false;
	}


	SetServiceState(EServiceState::Open);

	ServiceLog(L"Service::ServiceOpen() : service is %ws\n", StateToString());

	//mThreadManager->WorkThreads();

	DoWork();

	//TODO : MainThread
	//Tick

	return true;
}

void Service::ServiceClose()
{
	if (true == IsServiceOpen())
	{
		SetServiceState(EServiceState::Close);

		mSessionManager->Shutdown();

		mListener->Shutdown();

		mIOCPServer->Shutdown();

		mThreadManager->Shutdown();

		mLoggerManager->Shutdown();

	}

	ServiceLog(L"Service::ServiceClose() : service is %ws\n", StateToString());
}

bool Service::Prepare()
{

	if (mLoggerManager == nullptr || false == mLoggerManager->Prepare(shared_from_this()))
	{
		ServiceLog(L"Service::SetLoggerManager() : failed to prepare for logger manager\n");
		return false;
	}

	if (mThreadManager == nullptr || false == mThreadManager->Prepare(shared_from_this()))
	{
		ServiceLog(L"Service::SetThreadManager() : failed to prepare for thread manager\n");
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

void Service::SetServiceState(const EServiceState state)
{
	mServiceState = state;
}

bool Service::IsServiceOpen() const
{
	return mServiceState == EServiceState::Open ? true : false;
}

const WCHAR* Service::StateToString() const
{
	switch (mServiceState)
	{
	case EServiceState::Open:
		return L"Open";
		break;
	case EServiceState::Close:
		return L"Close";
		break;
	case EServiceState::Connecting:
		return L"Connecting";
		break;
	case EServiceState::Executing:
		return L"Executing";
		break;
	case EServiceState::Fetching:
		return L"Fetching";
		break;
	default:
		return L"Error";
		break;
	}

	return L"Error";
}

EServiceState Service::GetState() const
{
	return mServiceState;
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

void Service::ServiceLog(const WCHAR* log, ...)
{
	mLoggerManager->WriteLog(log);
}
