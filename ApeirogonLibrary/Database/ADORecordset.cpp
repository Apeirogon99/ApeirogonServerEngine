#include "pch.h"
#include "ADORecordset.h"
#include "ADOCommand.h"

ADORecordset::ADORecordset()
{
	Initlialze();
	//SetActiveEvent();
}

ADORecordset::~ADORecordset()
{
	//SetDeactiveateEvent();
	UnInitlialze();
}

ADORecordset::ADORecordset(_RecordsetPtr recordset)
{
	Initlialze();
	//SetActiveEvent();

	if (recordset)
	{
		mRefCount++;
		Attach(recordset, true);
	}

}

ADORecordset& ADORecordset::operator=(_RecordsetPtr recordset)
{
	Initlialze();
	//SetActiveEvent();

	if (recordset)
	{
		mRefCount++;
		Attach(recordset, true);
	}

	return *this;
}

ADORecordset::ADORecordset(const ADORecordset& inRecordset)
{

	mRefCount = inRecordset.mRefCount;

	_RecordsetPtr tempRecorset = inRecordset.GetInterfacePtr();
	if (tempRecorset)
	{
		mRefCount++;
		Attach(tempRecorset, true);
	}

	mConnectionPointContainer	= inRecordset.mConnectionPointContainer;
	mConnectionPoint			= inRecordset.mConnectionPoint;
	mRecordsetEvent				= inRecordset.mRecordsetEvent;
	mUnknown					= inRecordset.mUnknown;
	mRstEvt						= inRecordset.mRstEvt;
	mConnectionInfo = inRecordset.mConnectionInfo;
}

ADORecordset& ADORecordset::operator=(const ADORecordset& inRecordset)
{

	mRefCount = inRecordset.mRefCount;

	_RecordsetPtr tempRecorset = inRecordset.GetInterfacePtr();
	if (tempRecorset)
	{
		mRefCount++;
		Attach(tempRecorset, true);
	}

	mConnectionPointContainer = inRecordset.mConnectionPointContainer;
	mConnectionPoint = inRecordset.mConnectionPoint;
	mRecordsetEvent = inRecordset.mRecordsetEvent;
	mUnknown = inRecordset.mUnknown;
	mRstEvt = inRecordset.mRstEvt;
	mConnectionInfo = inRecordset.mConnectionInfo;

	return *this;
}

void ADORecordset::Initlialze()
{
	HRESULT hResult = S_FALSE;

	// CreateInstance
	hResult = this->CreateInstance(__uuidof(Recordset));
	if (FAILED(hResult))
	{
		return;
	}

	auto recordsetInterface = this->GetInterfacePtr();
	if (!recordsetInterface)
	{
		return;
	}

	//DefaultSetting
	mRefCount = 0;
}

void ADORecordset::UnInitlialze()
{
	mRefCount--;
	//Close();
}

void ADORecordset::Open()
{
	HRESULT	hResult = S_FALSE;
	auto recordsetInterface = this->GetInterfacePtr();
	if (!recordsetInterface)
	{
		return;
	}

	recordsetInterface->CursorLocation	= adUseClient;
	recordsetInterface->CursorType		= adOpenStatic;
	recordsetInterface->LockType		= adLockBatchOptimistic;

	ADOVariant		commandText			= recordsetInterface->GetSource();
	ADOVariant		activeConnection	= recordsetInterface->GetActiveConnection();
	CursorTypeEnum	cursorType			= CursorTypeEnum::adOpenForwardOnly;
	LockTypeEnum	lockType			= LockTypeEnum::adLockReadOnly;
	long			option				= CommandTypeEnum::adCmdText;

	hResult = recordsetInterface->Open("EXEC dbo.load_character_sp", activeConnection.mVar, cursorType, lockType, option);
}

void ADORecordset::Open(class ADOCommand& inCommand, class ADOConnection& inConnection)
{
	HRESULT	hResult = S_FALSE;
	auto recordsetInterface = this->GetInterfacePtr();
	if (!recordsetInterface)
	{
		return;
	}

	recordsetInterface->CursorLocation	= adUseClient;
	recordsetInterface->CursorType		= CursorTypeEnum::adOpenForwardOnly;
	recordsetInterface->LockType		= LockTypeEnum::adLockReadOnly;

	_variant_t		commandText			= inCommand.GetCommandSource().mVar;
	_variant_t		activeConnection	= inConnection.GetConnectionInfo().ToString();
	CursorTypeEnum	cursorType			= CursorTypeEnum::adOpenForwardOnly;
	LockTypeEnum	lockType			= LockTypeEnum::adLockReadOnly;
	long			option				= CommandTypeEnum::adCmdText;
	
	try
	{
		hResult = recordsetInterface->Open(commandText, activeConnection, cursorType, lockType, option);
	}
	catch (_com_error& error)
	{
		const TCHAR* msg = error.ErrorMessage();
		std::wcout << msg << std::endl;
	}
}

bool ADORecordset::IsOpen() const
{
	auto recordsetInterface = this->GetInterfacePtr();
	if (!recordsetInterface)
	{
		return false;
	}

	long state = recordsetInterface->GetState();
	return (state == ObjectStateEnum::adStateOpen) ? true : false;
}

void ADORecordset::Close()
{
	auto recordsetInterface = this->GetInterfacePtr();
	if (!recordsetInterface)
	{
		return;
	}

	if (IsOpen())
	{
		recordsetInterface->Close();
	}
}

void ADORecordset::SetAsync()
{
	auto recordsetInterface = this->GetInterfacePtr();
	if (!recordsetInterface)
	{
		return;
	}
}

void ADORecordset::MoveFirst()
{
	HRESULT hResult = S_FALSE;

	auto recordsetInterface = this->GetInterfacePtr();
	if (!recordsetInterface)
	{
		return;
	}

	hResult = recordsetInterface->MoveFirst();
	if (FAILED(hResult))
	{
		return;
	}
}

void ADORecordset::MoveLast()
{
	HRESULT hResult = S_FALSE;

	auto recordsetInterface = this->GetInterfacePtr();
	if (!recordsetInterface)
	{
		return;
	}

	hResult = recordsetInterface->MoveLast();
	if (FAILED(hResult))
	{
		return;
	}
}

void ADORecordset::MoveNext()
{
	HRESULT hResult = S_FALSE;

	auto recordsetInterface = this->GetInterfacePtr();
	if (!recordsetInterface)
	{
		return;
	}

	hResult = recordsetInterface->MoveNext();
	if (FAILED(hResult))
	{
		return;
	}
}

void ADORecordset::MovePrevious()
{
	HRESULT hResult = S_FALSE;

	auto recordsetInterface = this->GetInterfacePtr();
	if (!recordsetInterface)
	{
		return;
	}

	hResult = recordsetInterface->MovePrevious();
	if (FAILED(hResult))
	{
		return;
	}
}

bool ADORecordset::IsEof()
{
	auto recordsetInterface = this->GetInterfacePtr();
	if (!recordsetInterface)
	{
		return false;
	}

	VARIANT_BOOL isEnd = recordsetInterface->GetadoEOF();
	return (isEnd == S_OK) ? false : true;
}

ADOVariant ADORecordset::GetFieldItem(const WCHAR* filedName)
{
	auto recordsetInterface = this->GetInterfacePtr();
	if (!recordsetInterface)
	{
		return NULL;
	}

	auto fieldInterface = recordsetInterface->Fields;
	if (!fieldInterface)
	{
		return NULL;
	}

	auto item = fieldInterface->Item[filedName];
	if (!item)
	{
		return NULL;
	}

	_variant_t tempParam = item->GetValue();
	return tempParam;
}

void ADORecordset::SetConnectionInfo(ADOConnectionInfo& inConnectionInfo)
{
	mConnectionInfo = inConnectionInfo;
}

void ADORecordset::SetActiveEvent()
{

	// Start using the Recordset events  
	HRESULT hResult;
	mConnectionPointContainer = NULL;
	mConnectionPoint = NULL;
	mRecordsetEvent = NULL;
	mUnknown = NULL;

	hResult = this->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&mConnectionPointContainer);

	if (FAILED(hResult))
		return;

	hResult = mConnectionPointContainer->FindConnectionPoint(__uuidof(RecordsetEvents), &mConnectionPoint);
	mConnectionPointContainer->Release();

	if (FAILED(hResult))
		return;

	mRecordsetEvent = new ADORecordsetEvent();
	hResult = mRecordsetEvent->QueryInterface(__uuidof(IUnknown), (void**)&mUnknown);

	if (FAILED(hResult))
		return;

	hResult = mConnectionPoint->Advise(mUnknown, &mRstEvt);
	mConnectionPoint->Release();

	if (FAILED(hResult))
		return;
}

void ADORecordset::SetDeactiveateEvent()
{

	// Stop using the Recordset events  
	HRESULT hResult;
	hResult = this->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&mConnectionPointContainer);

	if (FAILED(hResult))
		return;

	hResult = mConnectionPointContainer->FindConnectionPoint(__uuidof(RecordsetEvents), &mConnectionPoint);
	mConnectionPoint->Release();

	if (FAILED(hResult))
		return;

	hResult = mConnectionPoint->Unadvise(mRstEvt);
	mConnectionPoint->Release();

	if (FAILED(hResult))
		return;
}
