#pragma once

class SessionManager : public std::enable_shared_from_this<SessionManager>
{
public:
	APEIROGON_API SessionManager(const SessionFactory& sessionFactory, const uint32 maxSessionCount, const uint32 inBufferSize);
	APEIROGON_API virtual ~SessionManager();

	SessionManager(const SessionManager& sessionManager) = delete;
	SessionManager& operator=(const SessionManager& sessionManager) = delete;

public:
	bool		Prepare(const ServicePtr& service);
	SessionPtr	CreateSession();
	bool		InsertSession(const SessionPtr& session);
	bool		ReleaseSession(const SessionPtr& session);
	bool		FindSession(const SessionPtr& session);
	void		BroadCastSession(SendBufferPtr sendBuffer);
	void		Shutdown();

public:
	
	uint32		GetSessionCount() const;
	uint32		GetMaxSessionCount() const;
	APEIROGON_API ServicePtr GetService() const;

public:
	APEIROGON_API void SessionManagerLog(const WCHAR* log, ...);

protected:
	uint32					mSessionCount;
	uint32					mMaxSessionCount;
	uint32					mMaxBufferSize;
	std::set<SessionPtr>	mSessions;
	SessionFactory			mSessionFactory;
	ServicePtr				mService;
	FastSpinLock			mFastSpinLock;
	std::mutex				mLock;
};