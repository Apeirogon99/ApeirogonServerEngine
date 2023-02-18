#include "pch.h"
#include "SessionManager.h"

SessionManager::SessionManager(const SessionFactory& sessionFactory, const uint32 maxSessionCount) :mSessionFactory(sessionFactory), mMaxSessionCount(maxSessionCount), mSessionCount(0)
{
	
}

SessionManager::~SessionManager()
{
	wprintf(L"SessionManager::~SessionManager()\n");
}

bool SessionManager::Prepare(const ServicePtr& service)
{
	mService = service;
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

	mSendRingBuffer.SetBufferSize(1024);

	return true;
}

SessionPtr SessionManager::CreateSession()
{
	FastLockGuard lockGuard(mFastSpinLock);

	if (mSessionFactory == nullptr)
	{
		SessionManagerLog(L"[SessionManager::CreateSession()] Is not valid SessionFactory\n");
		return nullptr;
	}

	SessionPtr session = mSessionFactory();
	session->SetSessionManager(weak_from_this());

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

	mSessionCount++;

	SessionManagerLog(L"[SessionManager::InsertSession()] (CUR : %ld)\n", mSessionCount);

	return true;
}

bool SessionManager::ReleaseSession(const SessionPtr& session)
{
	FastLockGuard lockGuard(mFastSpinLock);

	size_t result;
	result = mSessions.erase(session);
	if (result == 0)
	{
		SessionManagerLog(L"[SessionManager::ReleaseSession()] Is not valid session");
		return false;
	}

	mSessionCount--;

	SessionManagerLog(L"[SessionManager::ReleaseSession()] (CUR : %ld)\n", mSessionCount);
	
	return true;
}

bool SessionManager::FindSession(const SessionPtr& session)
{
	FastLockGuard lockGuard(mFastSpinLock);
	auto findSession = mSessions.find(session);
	return findSession != mSessions.end() ? true : false;
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

	mService.reset();

	wprintf(L"SessionManager::Shutdown() : session manager successfully shutdown\n");
}

uint32 SessionManager::GetSessionCount() const
{
	return mSessionCount;
}

uint32 SessionManager::GetMaxSessionCount() const
{
	return mMaxSessionCount;
}

SendRingBuffer& SessionManager::GetSendRingBuffer()
{
	return mSendRingBuffer;
}

void SessionManager::SessionManagerLog(const WCHAR* log, ...)
{
#if _DEBUG
	mService->GetLoggerManager()->WriteLog(log);
#endif
}
