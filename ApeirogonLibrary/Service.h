#pragma once

enum class EServiceState
{
	Open,
	Close,
	Connecting,
	Executing,
	Fetching
};

class Service : public std::enable_shared_from_this<Service>
{
public:
	APEIROGON_API Service();
	APEIROGON_API virtual ~Service();

	Service(const Service& service) = delete;
	Service& operator=(const Service& service) = delete;

public:
	APEIROGON_API bool			ServiceOpen();
	APEIROGON_API void			ServiceClose();

public:
	APEIROGON_API virtual void	Tick(const float deltaTime) abstract;
	void						DoWork();

protected:
	//Setting
	APEIROGON_API bool			Prepare();
	APEIROGON_API virtual bool	SettingService() abstract;
	APEIROGON_API bool			SetIOCPServer(IOCPServerPtr& iocpServer);
	APEIROGON_API bool			SetListener(ListenerPtr& listener);
	APEIROGON_API bool			SetSessionManager(SessionManagerPtr& sessionManager);
	APEIROGON_API bool			SetThreadManager(ThreadManagerPtr& threadManager);
	APEIROGON_API bool			SetLoggerManager(LoggerManagerPtr& loggerManager);

	void						SetServiceState(const EServiceState state);

public:
	bool						IsServiceOpen() const;
	const WCHAR*				StateToString() const;
	EServiceState				GetState() const;
	SessionManagerPtr			GetSessionManager() const;
	ListenerPtr					GetListener() const;
	IOCPServerPtr				GetIOCPServer() const;
	ThreadManagerPtr			GetThreadManager() const;
	LoggerManagerPtr			GetLoggerManager() const;

protected:
	APEIROGON_API void			ServiceLog(const WCHAR* log, ...);

private:
	EServiceState				mServiceState;
	SessionManagerPtr			mSessionManager;
	ListenerPtr					mListener;
	IOCPServerPtr				mIOCPServer;
	ThreadManagerPtr			mThreadManager;
	LoggerManagerPtr			mLoggerManager;
};