#pragma once

class ADOConnectionEvents;

class ADOConnection : public _ConnectionPtr
{
public:
	ADO_API ADOConnection();
	ADO_API ~ADOConnection();
	ADO_API ADOConnection(const ADOConnection& connection);
	ADO_API ADOConnection& operator=(const ADOConnection& connection);

public:
	ADO_API void Open(const ADOConnectionInfo& connInfo);
	ADO_API void OpenEx(const ADOConnectionInfo& connInfo);
	ADO_API bool IsOpen() const;
	ADO_API void Close();

	ADO_API void SetActiveEvent();
	ADO_API void SetDeactiveateEvent();

	ADO_API const WCHAR* GetDatabaseName() const;
	ADO_API ADOConnectionInfo GetConnectionInfo() const;
	ADO_API void SetConnectionInfo(const ADOConnectionInfo& info);

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

