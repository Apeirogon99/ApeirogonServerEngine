#include "pch.h"
#include "ADOEvent.h"

ADOConnectionEvents::ADOConnectionEvents()
{
	mRef = 0;
}

ADOConnectionEvents::~ADOConnectionEvents()
{
}

HRESULT STDMETHODCALLTYPE ADOConnectionEvents::QueryInterface(REFIID riid, void** ppv)
{
    *ppv = NULL;
    if (riid == __uuidof(IUnknown) || riid == __uuidof(ConnectionEventsVt))
        *ppv = this;

    if (*ppv == NULL)
        return ResultFromScode(E_NOINTERFACE);

    AddRef();
    return NOERROR;
}

ULONG STDMETHODCALLTYPE ADOConnectionEvents::AddRef(void)
{
    return ++mRef;
}

ULONG STDMETHODCALLTYPE ADOConnectionEvents::Release(void)
{
    if (0 != --mRef)
        return mRef;
    delete this;
    return 0;
}

STDMETHODIMP ADOConnectionEvents::raw_InfoMessage(Error* pError, EventStatusEnum* adStatus, _Connection* pConnection)
{
    *adStatus = adStatusUnwantedEvent;
    return S_OK;
}

STDMETHODIMP ADOConnectionEvents::raw_BeginTransComplete(LONG TransactionLevel, Error* pError, EventStatusEnum* adStatus, _Connection* pConnection)
{
    *adStatus = adStatusUnwantedEvent;
    return S_OK;
}

STDMETHODIMP ADOConnectionEvents::raw_CommitTransComplete(Error* pError, EventStatusEnum* adStatus, _Connection* pConnection)
{
    *adStatus = adStatusUnwantedEvent;
    return S_OK;
}

STDMETHODIMP ADOConnectionEvents::raw_RollbackTransComplete(Error* pError, EventStatusEnum* adStatus, _Connection* pConnection)
{
    *adStatus = adStatusUnwantedEvent;
    return S_OK;
}

STDMETHODIMP ADOConnectionEvents::raw_WillExecute(BSTR* Source, CursorTypeEnum* CursorType, LockTypeEnum* LockType, long* Options, EventStatusEnum* adStatus, _Command* pCommand, _Recordset* pRecordset, _Connection* pConnection)
{
    *adStatus = adStatusUnwantedEvent;
    return S_OK;
}

STDMETHODIMP ADOConnectionEvents::raw_ExecuteComplete(LONG RecordsAffected, Error* pError, EventStatusEnum* adStatus, _Command* pCommand, _Recordset* pRecordset, _Connection* pConnection)
{
    *adStatus = adStatusUnwantedEvent;
    return S_OK;
}

STDMETHODIMP ADOConnectionEvents::raw_WillConnect(BSTR* ConnectionString, BSTR* UserID, BSTR* Password, long* Options, EventStatusEnum* adStatus, _Connection* pConnection)
{
    *adStatus = adStatusUnwantedEvent;
    return S_OK;
}

STDMETHODIMP ADOConnectionEvents::raw_ConnectComplete(Error* pError, EventStatusEnum* adStatus, _Connection* pConnection)
{
    *adStatus = adStatusUnwantedEvent;
    return S_OK;
}

STDMETHODIMP ADOConnectionEvents::raw_Disconnect(EventStatusEnum* adStatus, _Connection* pConnection)
{
    *adStatus = adStatusUnwantedEvent;
    return S_OK;
}

ADORecordsetEvent::ADORecordsetEvent()
{
    mRef = 0;
}

ADORecordsetEvent::~ADORecordsetEvent()
{
}

HRESULT STDMETHODCALLTYPE ADORecordsetEvent::QueryInterface(REFIID riid, void** ppv)
{
	*ppv = NULL;
	if (riid == __uuidof(IUnknown) || riid == __uuidof(ConnectionEventsVt))
		*ppv = this;

	if (*ppv == NULL)
		return ResultFromScode(E_NOINTERFACE);

	AddRef();
	return NOERROR;
}

ULONG STDMETHODCALLTYPE ADORecordsetEvent::AddRef()
{
	return ++mRef;
}

ULONG STDMETHODCALLTYPE ADORecordsetEvent::Release()
{
	if (0 != --mRef)
		return mRef;
	delete this;
	return 0;
}

STDMETHODIMP ADORecordsetEvent::raw_WillChangeField(LONG cFields, VARIANT Fields, EventStatusEnum* adStatus, _Recordset* pRecordset)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP ADORecordsetEvent::raw_FieldChangeComplete(LONG cFields, VARIANT Fields, Error* pError, EventStatusEnum* adStatus, _Recordset* pRecordset)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP ADORecordsetEvent::raw_WillChangeRecord(EventReasonEnum adReason, LONG cRecords, EventStatusEnum* adStatus, _Recordset* pRecordset)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP ADORecordsetEvent::raw_RecordChangeComplete(EventReasonEnum adReason, LONG cRecords, Error* pError, EventStatusEnum* adStatus, _Recordset* pRecordset)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP ADORecordsetEvent::raw_WillChangeRecordset(EventReasonEnum adReason, EventStatusEnum* adStatus, _Recordset* pRecordset)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP ADORecordsetEvent::raw_RecordsetChangeComplete(EventReasonEnum adReason, Error* pError, EventStatusEnum* adStatus, _Recordset* pRecordset)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP ADORecordsetEvent::raw_WillMove(EventReasonEnum adReason, EventStatusEnum* adStatus, _Recordset* pRecordset)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP ADORecordsetEvent::raw_MoveComplete(EventReasonEnum adReason, Error* pError, EventStatusEnum* adStatus, _Recordset* pRecordset)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP ADORecordsetEvent::raw_EndOfRecordset(VARIANT_BOOL* fMoreData, EventStatusEnum* adStatus, _Recordset* pRecordset)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP ADORecordsetEvent::raw_FetchProgress(long Progress, long MaxProgress, EventStatusEnum* adStatus, _Recordset* pRecordset)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP ADORecordsetEvent::raw_FetchComplete(Error* pError, EventStatusEnum* adStatus, _Recordset* pRecordset)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}