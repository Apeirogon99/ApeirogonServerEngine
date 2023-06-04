#pragma once
class ThreadManager
{
public:
	APEIROGON_API ThreadManager(const uint32 maxThreadCount, const uint32 inMaxThreadTimeOut);
	APEIROGON_API virtual ~ThreadManager();

	ThreadManager(const ThreadManager& threadManager) = delete;
	ThreadManager& operator=(const ThreadManager& threadManager) = delete;

public:
	bool		Prepare(ServicePtr service);
	void		Shutdown();

public:
	void		DoWorkThreads(const uint32 inTimeOut);
	void		StopWorkThreads();

	static void InitTLS();
	static void DestroyTLS();
	
protected:
	void		ThreadLog(const WCHAR* log, ...);

private:
	uint32						maxThreadCount;
	uint32						maxThreadTimeOut;
	std::vector<std::thread>	mThreads;
	ServicePtr					mService;
	TimeStamp					mThreadProcessTime;
};

