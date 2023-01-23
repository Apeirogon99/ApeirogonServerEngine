#include "pch.h"
#include "ADORecordset.h"

ADORecordset::ADORecordset()
{
	Initlialze();
	SetActiveEvent();
}

ADORecordset::~ADORecordset()
{
	SetDeactiveateEvent();
	UnInitlialze();
}

ADORecordset::ADORecordset(_RecordsetPtr recordset)
{
	if (recordset)
		Attach(recordset, true);
}

ADORecordset& ADORecordset::operator=(_RecordsetPtr recordset)
{
	if (recordset)
		Attach(recordset, true);

	return *this;
}

void ADORecordset::Initlialze()
{
	HRESULT hResult = S_FALSE;

	hResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hResult))
	{
		return;
	}

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
	
}

void ADORecordset::UnInitlialze()
{
	Close();
	CoUninitialize();
}

void ADORecordset::Open()
{
	HRESULT					hResult = S_FALSE;
	auto recordsetInterface = this->GetInterfacePtr();
	if (!recordsetInterface)
	{
		return;
	}

	_variant_t		commandText = recordsetInterface->GetSource();
	_variant_t		activeConnectString = recordsetInterface->GetActiveConnection();
	CursorTypeEnum	cursorType = CursorTypeEnum::adOpenForwardOnly;
	LockTypeEnum	lockType = LockTypeEnum::adLockReadOnly;
	CommandTypeEnum option = CommandTypeEnum::adCmdStoredProc;

	hResult = recordsetInterface->Open(commandText, activeConnectString, cursorType, lockType, option);
}

bool ADORecordset::IsOpen()
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
		return S_FALSE;
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

void ADORecordset::SetActiveEvent()
{
	if (mIsEvent == true)
	{
		return;
	}
	mIsEvent = true;

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
	if (mIsEvent == false)
	{
		return;
	}
	mIsEvent = false;

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
