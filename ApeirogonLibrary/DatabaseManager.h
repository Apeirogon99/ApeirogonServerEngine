#pragma once

class DatabaseManager
{
public:
	APEIROGON_API DatabaseManager(const uint32 inThreadPoolSize, const uint32 inDatabasePoolSize);
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
	APEIROGON_API virtual void InitializeDatabase() abstract;
	APEIROGON_API void PushConnectionPool(ADOConnection& inConnection, const ADOConnectionInfo& inConnectioninfo);
	APEIROGON_API void PrintConnectionPoolState();
	APEIROGON_API void KeepConnection();

	APEIROGON_API bool PushAsyncTaskQueue(PacketSessionPtr& inSession, ADOConnection& inADOConnection, ADOCommand& inADOCommand, ADORecordset& inADORecordset, ADOCallBack& inADOCallBack);

public:
	void	DoWorkThreads();
	int64	ProcessTask();

protected:
	void DatabaseLog(const WCHAR* log, ...);

private:
	uint32							mConnectionPoolSize;
	uint32							mConnectionUsedSize;

	ServicePtr						mService;
	ADOConnection*					mConnections;
	ADOConnectionInfo*				mConnectionInfos;

	LockController					mLockController;
	CircularQueue<ADOAsyncTaskPtr>	mAsyncTaskQueue;
	CircularQueue<ADOAsyncTaskPtr>	mDatabaseTaskQueue;

	uint32							mThreadPoolSize;
	std::vector<std::thread>		mThreads;

	TimeStamp						mProcessTimeStamp;
};

