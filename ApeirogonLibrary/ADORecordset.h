#pragma once

class ADORecordsetEvent;

class ADORecordset : public _RecordsetPtr
{
public:
	APEIROGON_API ADORecordset();
	APEIROGON_API ~ADORecordset();

	APEIROGON_API ADORecordset(_RecordsetPtr recordset);
	APEIROGON_API ADORecordset& operator=(_RecordsetPtr recordset);

	APEIROGON_API ADORecordset(const ADORecordset& inRecordset);
	APEIROGON_API ADORecordset& operator=(const ADORecordset& inRecordset);

protected:
	ADORecordset(ADORecordset&&) = delete;
	ADORecordset& operator=(ADORecordset&&) = delete;

public:
	APEIROGON_API void			Open();
	APEIROGON_API bool			IsOpen();
	APEIROGON_API bool			IsComplete();
	APEIROGON_API void			Close();
	APEIROGON_API void			SetAsync();

	APEIROGON_API void			MoveFirst();
	APEIROGON_API void			MoveLast();
	APEIROGON_API void			MoveNext();
	APEIROGON_API void			MovePrevious();

	APEIROGON_API bool			IsEof();
	APEIROGON_API ADOVariant	GetFieldItem(const WCHAR* filedName);

	APEIROGON_API void			SetActiveEvent();
	APEIROGON_API void			SetDeactiveateEvent();

protected:
	void Initlialze();
	void UnInitlialze();

private:
	ADORecordsetEvent*			mRecordsetEvent;
	IConnectionPointContainer*	mConnectionPointContainer;
	IConnectionPoint*			mConnectionPoint;
	IUnknown*					mUnknown;
	DWORD						mRstEvt;

	bool mIsEvent;
};

