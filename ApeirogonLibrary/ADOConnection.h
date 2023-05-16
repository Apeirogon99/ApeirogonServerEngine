#pragma once

class ADOConnectionEvents;

class ADOConnection : public _ConnectionPtr
{
public:
	APEIROGON_API ADOConnection();
	APEIROGON_API ~ADOConnection();
	APEIROGON_API ADOConnection(const ADOConnection& connection);
	APEIROGON_API ADOConnection& operator=(const ADOConnection& connection);

public:
	APEIROGON_API void Open(const ADOConnectionInfo& connInfo);
	APEIROGON_API void OpenEx(const ADOConnectionInfo& connInfo);
	APEIROGON_API bool IsOpen() const;
	APEIROGON_API void Close();

	APEIROGON_API void SetActiveEvent();
	APEIROGON_API void SetDeactiveateEvent();

	APEIROGON_API const WCHAR* GetDatabaseName() const;
	APEIROGON_API ADOConnectionInfo GetConnectionInfo() const;
	APEIROGON_API void SetConnectionInfo(const ADOConnectionInfo& info);
	APEIROGON_API _bstr_t GetConnectionString();

protected:
	void Initlialze();
	void UnInitlialze();

private:
	ADOConnectionEvents*		mConnectionEvent;
	IConnectionPointContainer*	mConnectionPointContainer;
	IConnectionPoint*			mConnectionPoint;
	IUnknown*					mUnknown;
	DWORD						mConnEvt;

	ADOConnectionInfo			mConnectionInfo;
	bool mIsEvent;
};

