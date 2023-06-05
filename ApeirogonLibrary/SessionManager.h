#pragma once

class SessionManager : public std::enable_shared_from_this<SessionManager>
{
public:
	APEIROGON_API SessionManager(const SessionFactory& sessionFactory, const uint32 maxSessionCount, const uint32 inBufferSize);
	APEIROGON_API virtual ~SessionManager();

	SessionManager(const SessionManager& sessionManager) = delete;
	SessionManager& operator=(const SessionManager& sessionManager) = delete;

public:
	bool						Prepare(ServicePtr service);
	void						Shutdown();

	SessionPtr					CreateSession();
	bool						InsertSession(const SessionPtr& session);
	APEIROGON_API bool			ReleaseSession(const SessionPtr& session);
	APEIROGON_API bool			FindSession(const SessionPtr& session);
	APEIROGON_API void			BroadCastSession(SendBufferPtr sendBuffer);

public:
	uint32						GetSessionCount() const;
	uint32						GetMaxSessionCount() const;
	uint32						GetMaxBufferSize() const;
	APEIROGON_API int64			GetServiceTimeStamp() const;
	APEIROGON_API ServicePtr	GetService() const;

public:
	void WorkDispatch();

public:
	APEIROGON_API void SessionManagerLog(const WCHAR* log, ...);

protected:
	uint32							mSessionCount;
	uint32							mMaxSessionCount;
	uint32							mMaxBufferSize;
	std::set<SessionPtr>			mSessions;
	SessionFactory					mSessionFactory;
	ServicePtr						mService;
	FastSpinLock					mFastSpinLock;
};