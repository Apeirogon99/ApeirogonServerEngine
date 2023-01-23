#pragma once
class ThreadManager
{
public:
	APEIROGON_API ThreadManager(const uint32 maxThreadCount);
	APEIROGON_API virtual ~ThreadManager();

	ThreadManager(const ThreadManager& threadManager) = delete;
	ThreadManager& operator=(const ThreadManager& threadManager) = delete;

public:
	bool		Prepare(const ServicePtr& service);
	void		Shutdown();

public:
	void		WorkThreads();
	void		JoinThreads();

	static void InitTLS();
	static void DestroyTLS();
	
protected:
	void		ThreadLog(const WCHAR* log, ...);

private:
	uint32						maxThreadCount;
	std::vector<std::thread>	mThreads;
	ServicePtr					mService;
};

