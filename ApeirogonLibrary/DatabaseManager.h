#pragma once

class DatabaseManager
{
public:
	APEIROGON_API DatabaseManager(const size_t inThreadPoolSize, const size_t inDatabasePoolSize);
	APEIROGON_API virtual ~DatabaseManager();

protected:
	DatabaseManager(const DatabaseManager&) = delete;
	DatabaseManager(DatabaseManager&&) = delete;

	DatabaseManager operator=(const DatabaseManager&) = delete;
	DatabaseManager operator=(DatabaseManager&&) = delete;

public:
	bool Prepare(ServicePtr service);
	void Shutdown();

public:
	APEIROGON_API void PushConnectionPool(ADOConnection& inConnection, const ADOConnectionInfo& inConnectioninfo);
	APEIROGON_API void PrintConnectionPoolState();
	APEIROGON_API DatabaseTaskQueuePtr GetDatabaseTaskQueue();

	void ProcessDatabaseTask(const int64 inServiceTimeStamp);
	void WorkDispatch();

protected:
	APEIROGON_API virtual void InitializeDatabase() abstract;

	bool StartDatabaseManager();
	void DatabaseLoop();
	void KeepConnection();
	void DatabaseLog(const WCHAR* log, ...);

private:
	size_t						mPoolSize;
	size_t						mUsedSize;

	ServicePtr					mService;
	ADOConnection*				mConnections;
	ADOConnectionInfo*			mConnectionInfos;

	bool						mIsRunning;
	TimeStamp					mTimeStamp;
	std::thread					mDatabaseManagerThread;
	DatabaseTaskQueuePtr		mDatabaseHandler;
};

