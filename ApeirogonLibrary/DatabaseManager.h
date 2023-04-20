#pragma once

class DatabaseManager
{
public:
	APEIROGON_API DatabaseManager(const size_t poolSize = 10);
	APEIROGON_API virtual ~DatabaseManager();

protected:
	DatabaseManager(const DatabaseManager&) = delete;
	DatabaseManager(DatabaseManager&&) = delete;

	DatabaseManager operator=(const DatabaseManager&) = delete;
	DatabaseManager operator=(DatabaseManager&&) = delete;

public:
	bool Prepare(const ServicePtr& service);
	void Shutdown();

public:
	APEIROGON_API void PushConnectionPool(ADOConnection& inConnection, const ADOConnectionInfo& inConnectioninfo);
	APEIROGON_API bool StartDatabaseManager();
	APEIROGON_API void PrintConnectionPoolState();

	void DatabaseLoop();

	APEIROGON_API ADOTask& GetTask();

protected:
	APEIROGON_API virtual void InitializeDatabase() abstract;
	void KeepConnection();
	void DatabaseLog(const WCHAR* log, ...);

private:
	size_t						mPoolSize;
	size_t						mUsedSize;

	ServicePtr					mService;
	ADOConnection*				mConnections;
	ADOConnectionInfo*			mConnectionInfos;

	bool						mIsRunning;
	std::thread					mDatabaseManagerThread;
	ADOTask						mADOTask;
};

