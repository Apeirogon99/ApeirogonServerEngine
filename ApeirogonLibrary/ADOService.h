#pragma once
class ADOService
{
public:
	ADO_API ADOService(const size_t poolSize = 10);
	ADO_API ~ADOService();

	ADO_API bool Run();
	ADO_API void PushConnectionPool(ADOConnection& conn, const ADOConnectionInfo& info);
	ADO_API void PrintConnectionPoolState();
	ADO_API bool WaitConnectionPool();

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

