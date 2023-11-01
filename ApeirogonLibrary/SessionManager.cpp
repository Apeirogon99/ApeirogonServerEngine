#include "pch.h"
#include "SessionManager.h"

SessionManager::SessionManager(const SessionFactory& sessionFactory, const uint32 maxSessionCount, const uint32 inBufferSize) :mSessionFactory(sessionFactory), mMaxSessionCount(maxSessionCount), mSessionCount(0), mMaxBufferSize(inBufferSize), mFastSpinLock(), mProcessTimeStamp(L"Dispatch")
{
	
}

SessionManager::~SessionManager()
{
	wprintf(L"[SessionManager::~SessionManager()]\n");
}

bool SessionManager::Prepare(ServicePtr service)
{
	this->mService = service;
	if (nullptr == mService)
	{
		return false;
	}

	if (nullptr == mSessionFactory)
	{
		return false;
	}

	if (mMaxSessionCount <= 0)
	{
		return false;
	}

	SessionManagerLog(L"[SessionManager::Prepare()] Set Session [MAX : %ld, BufferSize : %ld]\n", mMaxSessionCount, mMaxBufferSize);

	return true;
}

SessionPtr SessionManager::CreateSession(const ESessionMode& inSessionMode)
{

	FastLockGuard lockGuard(mFastSpinLock);

	if (mSessionFactory == nullptr)
	{
		SessionManagerLog(L"[SessionManager::CreateSession()] Is not valid SessionFactory\n");
		return nullptr;
	}

	SessionPtr session = mSessionFactory();
	if (false == session->Prepare(weak_from_this(), inSessionMode))
	{
		SessionManagerLog(L"[SessionManager::CreateSession()] failed to session prepare\n");
		return nullptr;
	}

	IOCPServerPtr iocpServer = mService->GetIOCPServer();
	WinSocketPtr sessionSocket = session->GetWinSocket();
	if (false == iocpServer->RegisterSocketToIOCP(sessionSocket))
	{
		SessionManagerLog(L"[SessionManager::CreateSession()] failed to register socket to iocp\n");
		return nullptr;
	}

	return session;
}

bool SessionManager::InsertSession(const SessionPtr& session)
{
	FastLockGuard lockGuard(mFastSpinLock);

	auto result = mSessions.insert(session);
	if (false == result.second)
	{
		SessionManagerLog(L"[SessionManager::InsertSession()] already have session");
		return false;
	}

	SessionManagerLog(L"[SessionManager::InsertSession()] (CUR : %ld)\n", ++mSessionCount);

	return true;
}

bool SessionManager::ReleaseSession(const SessionPtr& session)
{
	FastLockGuard lockGuard(mFastSpinLock);

	size_t result = mSessions.erase(session);
	if (result == 0)
	{
		SessionManagerLog(L"[SessionManager::ReleaseSession()] Is not valid session");
		return false;
	}

	SessionManagerLog(L"[SessionManager::ReleaseSession()] Success to release session (CUR : %ld)\n", --mSessionCount);
	
	return true;
}

bool SessionManager::FindSession(const SessionPtr& session)
{
	FastLockGuard lockGuard(mFastSpinLock);
	auto findSession = mSessions.find(session);
	return findSession != mSessions.end() ? true : false;
}

bool SessionManager::FindServerSession(SessionPtr& outServerSession)
{
	FastLockGuard lockGuard(mFastSpinLock);
	for (auto session : mSessions)
	{
		if (session->GetSessionMode() == ESessionMode::Server)
		{
			outServerSession = session;
			return true;
		}
	}
	return false;
}

void SessionManager::BroadCastSession(SendBufferPtr sendBuffer)
{
	FastLockGuard lockGuard(mFastSpinLock);

	for (SessionPtr session : mSessions)
	{
		session->Send(sendBuffer);
	}
}

void SessionManager::Shutdown()
{
	FastLockGuard lockGuard(mFastSpinLock);

	for (SessionPtr session : mSessions)
	{
		session->Shutdown();
		session.reset();
	}

	SessionManagerLog(L"[SessionManager::Shutdown()] Session manager successfully shutdown\n");
	mService.reset();
}

uint32 SessionManager::GetSessionCount() const
{
	return mSessionCount;
}

uint32 SessionManager::GetMaxSessionCount() const
{
	return mMaxSessionCount;
}

uint32 SessionManager::GetMaxBufferSize() const
{
	return mMaxBufferSize;
}

int64 SessionManager::GetServiceTimeStamp() const
{
	return mService->GetServiceTimeStamp();
}

ServicePtr SessionManager::GetService() const
{
	return mService;
}

int64 SessionManager::WorkDispatch()
{
	mProcessTimeStamp.StartTimeStamp();
	if (mSessions.empty())
	{
		return mProcessTimeStamp.GetTimeStamp();
	}

	FastLockGuard lockGuard(mFastSpinLock);

	for (auto curSession = mSessions.begin(); curSession != mSessions.end(); curSession++)
	{
		SessionPtr session = *curSession;
		if (nullptr == session)
		{
			continue;
		}

		int32 outPendingSize = 0;
		if (false == session->HasPending(outPendingSize))
		{
			continue;
		}


		RegisterSendEvent* registerSendEvent = new RegisterSendEvent;
		registerSendEvent->Init();
		registerSendEvent->owner = session;

		mService->GetIOCPServer()->PostDispatch(outPendingSize, registerSendEvent);

		//session->GetMonitoring().CheckSession();
	}

	return mProcessTimeStamp.GetTimeStamp();
}

void SessionManager::SessionManagerLog(const WCHAR* log, ...)
{
	mService->GetLoggerManager()->WriteLog(log);
}
