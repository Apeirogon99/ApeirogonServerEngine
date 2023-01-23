#pragma once

class ADORecordsetEvent;

class ADORecordset : public _RecordsetPtr
{
public:
	ADO_API ADORecordset();
	ADO_API ~ADORecordset();

	ADO_API ADORecordset(_RecordsetPtr recordset);
	ADORecordset& operator=(_RecordsetPtr recordset);

private:
	ADORecordset& operator=(const ADORecordset&) = delete;
	ADORecordset(const ADORecordset&) = delete;

public:
	ADO_API void			Open();
	ADO_API bool			IsOpen();
	ADO_API void			Close();

	ADO_API void			MoveFirst();
	ADO_API void			MoveLast();
	ADO_API void			MoveNext();
	ADO_API void			MovePrevious();

	ADO_API bool			IsEof();
	ADO_API ADOVariant		GetFieldItem(const WCHAR* filedName);

	ADO_API void			SetActiveEvent();
	ADO_API void			SetDeactiveateEvent();

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

