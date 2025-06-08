#pragma once

class ADOConnectionEvents : public ConnectionEventsVt
{
private:
	ULONG mRef;

public:
	ADOConnectionEvents();
	~ADOConnectionEvents();

protected:

public:

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override;
	virtual ULONG STDMETHODCALLTYPE Release(void) override;

	virtual STDMETHODIMP raw_InfoMessage(Error* pError, EventStatusEnum* adStatus, _Connection* pConnection) override;
	virtual STDMETHODIMP raw_BeginTransComplete(LONG TransactionLevel, Error* pError, EventStatusEnum* adStatus, _Connection* pConnection) override;
	virtual STDMETHODIMP raw_CommitTransComplete(Error* pError, EventStatusEnum* adStatus, _Connection* pConnection) override;
	virtual STDMETHODIMP raw_RollbackTransComplete(Error* pError, EventStatusEnum* adStatus, _Connection* pConnection) override;
	virtual STDMETHODIMP raw_WillExecute(BSTR* Source, CursorTypeEnum* CursorType, LockTypeEnum* LockType, long* Options, EventStatusEnum* adStatus, _Command* pCommand, _Recordset* pRecordset, _Connection* pConnection) override;
	virtual STDMETHODIMP raw_ExecuteComplete(LONG RecordsAffected, Error* pError, EventStatusEnum* adStatus, _Command* pCommand, _Recordset* pRecordset, _Connection* pConnection) override;
	virtual STDMETHODIMP raw_WillConnect(BSTR* ConnectionString, BSTR* UserID, BSTR* Password, long* Options, EventStatusEnum* adStatus, _Connection* pConnection) override;
	virtual STDMETHODIMP raw_ConnectComplete(Error* pError, EventStatusEnum* adStatus, _Connection* pConnection) override;
	virtual STDMETHODIMP raw_Disconnect(EventStatusEnum* adStatus, _Connection* pConnection) override;
};

class ADORecordsetEvent : public RecordsetEventsVt
{
private:
	ULONG mRef;

public:
	ADORecordsetEvent();
	~ADORecordsetEvent();

public:

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override;
	virtual ULONG STDMETHODCALLTYPE AddRef() override;
	virtual ULONG STDMETHODCALLTYPE Release() override;
	;
	virtual STDMETHODIMP raw_WillChangeField(LONG cFields, VARIANT Fields, EventStatusEnum* adStatus, _Recordset* pRecordset) override;
	virtual STDMETHODIMP raw_FieldChangeComplete(LONG cFields, VARIANT Fields, Error* pError, EventStatusEnum* adStatus, _Recordset* pRecordset) override;
	virtual STDMETHODIMP raw_WillChangeRecord(EventReasonEnum adReason, LONG cRecords, EventStatusEnum* adStatus, _Recordset* pRecordset) override;
	virtual STDMETHODIMP raw_RecordChangeComplete(EventReasonEnum adReason, LONG cRecords, Error* pError, EventStatusEnum* adStatus, _Recordset* pRecordset) override;
	virtual STDMETHODIMP raw_WillChangeRecordset(EventReasonEnum adReason, EventStatusEnum* adStatus, _Recordset* pRecordset) override;
	virtual STDMETHODIMP raw_RecordsetChangeComplete(EventReasonEnum adReason, Error* pError, EventStatusEnum* adStatus, _Recordset* pRecordset) override;
	virtual STDMETHODIMP raw_WillMove(EventReasonEnum adReason, EventStatusEnum* adStatus, _Recordset* pRecordset) override;
	virtual STDMETHODIMP raw_MoveComplete(EventReasonEnum adReason, Error* pError, EventStatusEnum* adStatus, _Recordset* pRecordset) override;
	virtual STDMETHODIMP raw_EndOfRecordset(VARIANT_BOOL* fMoreData, EventStatusEnum* adStatus, _Recordset* pRecordset) override;
	virtual STDMETHODIMP raw_FetchProgress(long Progress, long MaxProgress, EventStatusEnum* adStatus, _Recordset* pRecordset) override;
	virtual STDMETHODIMP raw_FetchComplete(Error* pError, EventStatusEnum* adStatus, _Recordset* pRecordset) override;
};