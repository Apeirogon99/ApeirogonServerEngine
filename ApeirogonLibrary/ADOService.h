#pragma once
class ADOService
{
public:
	APEIROGON_API ADOService(const size_t poolSize = 10);
	APEIROGON_API ~ADOService();

	APEIROGON_API bool Run();
	APEIROGON_API void PushConnectionPool(ADOConnection& conn, const ADOConnectionInfo& info);
	APEIROGON_API void PrintConnectionPoolState();
	APEIROGON_API bool WaitConnectionPool();

protected:
	void KeepConnection();
	void CloseAllConnection();
	void StartADO();
	void EndADO();



private:
	bool						mIsService;
	size_t						mPoolSize;
	std::vector<ADOConnection>	mConnections;
};

