#include "pch.h"
#include "ADOConnection.h"

ADOConnection::ADOConnection()
{
	Initlialze();
	//SetActiveEvent();
}

ADOConnection::~ADOConnection()
{
	//SetDeactiveateEvent();
	UnInitlialze();
}

ADOConnection::ADOConnection(const ADOConnection& inConnection)
{
	if (inConnection)
	{
		Initlialze();
		//SetActiveEvent();
		Attach(inConnection, true);

		mConnectionPointContainer = inConnection.mConnectionPointContainer;
		mConnectionPoint = inConnection.mConnectionPoint;
		mConnectionEvent = inConnection.mConnectionEvent;
		mUnknown = inConnection.mUnknown;
		mConnEvt = inConnection.mConnEvt;

		if (inConnection.mConnectionInfo.ToString() != nullptr)
		{
			mConnectionInfo = inConnection.mConnectionInfo;
		}
	}
}

ADOConnection& ADOConnection::operator=(const ADOConnection& inConnection)
{
	if (inConnection)
	{
		Initlialze();
		//SetActiveEvent();
		Attach(inConnection, true);

		mConnectionPointContainer	= inConnection.mConnectionPointContainer;
		mConnectionPoint			= inConnection.mConnectionPoint;
		mConnectionEvent			= inConnection.mConnectionEvent;
		mUnknown					= inConnection.mUnknown;
		mConnEvt					= inConnection.mConnEvt;

		if (inConnection.mConnectionInfo.ToString() != nullptr)
		{
			mConnectionInfo = inConnection.mConnectionInfo;
		}
	}

	return *this;
}

void ADOConnection::Open(const ADOConnectionInfo& connInfo)
{
	HRESULT hResult = S_FALSE;

	if (IsOpen())
	{
		return;
	}

	auto connectionInterface = this->GetInterfacePtr();
	if (!connectionInterface)
	{
		return;
	}

	this->SetConnectionInfo(connInfo);
	const WCHAR* connectionString = mConnectionInfo.ToString();
	connectionInterface->PutConnectionString(connectionString);

	const ConnectOptionEnum connectOption = ConnectOptionEnum::adConnectUnspecified;
	hResult = connectionInterface->Open(connectionString, "", "", adConnectUnspecified);

	if (FAILED(hResult))
	{
		return;
	}
}

void ADOConnection::OpenEx(const ADOConnectionInfo& connInfo)
{
	HRESULT hResult = S_FALSE;

	if (IsOpen())
	{
		return;
	}

	auto connectionInterface = this->GetInterfacePtr();
	if (!connectionInterface)
	{
		return;
	}

	this->SetConnectionInfo(connInfo);
	const WCHAR* connectionString = mConnectionInfo.ToString();
	connectionInterface->PutConnectionString(connectionString);

	const ConnectOptionEnum connectOption = ConnectOptionEnum::adAsyncConnect;
	hResult = connectionInterface->Open(connectionString, "", "", connectOption);
}

bool ADOConnection::IsOpen() const
{
	auto connectionInterface = this->GetInterfacePtr();
	if (!connectionInterface)
	{
		return false;
	}

	const long state = connectionInterface->GetState();

	return (state == ObjectStateEnum::adStateOpen) ? true : false;
}

void ADOConnection::Close()
{
	auto connectionInterface = this->GetInterfacePtr();
	if (!connectionInterface)
	{
		return;
	}

	if (IsOpen())
	{
		connectionInterface->Close();
		//connectionInterface->PutConnectionString("");
	}
}

void ADOConnection::SetActiveEvent()
{
	if (mIsEvent == true)
	{
		return;
	}
	mIsEvent = true;

	// Start using the Connection events
	HRESULT hResult;
	mConnectionPointContainer = NULL;
	mConnectionPoint = NULL;
	mConnectionEvent = NULL;
	mUnknown = NULL;

	hResult = this->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&mConnectionPointContainer);

	if (FAILED(hResult))
		return;

	hResult = mConnectionPointContainer->FindConnectionPoint(__uuidof(ConnectionEvents), &mConnectionPoint);
	mConnectionPointContainer->Release();

	if (FAILED(hResult))
		return;

	mConnectionEvent = new ADOConnectionEvents();
	hResult = mConnectionEvent->QueryInterface(__uuidof(IUnknown), (void**)&mUnknown);

	if (FAILED(hResult))
		return;

	hResult = mConnectionPoint->Advise(mUnknown, &mConnEvt);
	mConnectionPoint->Release();

	if (FAILED(hResult))
		return;
}

void ADOConnection::SetDeactiveateEvent()
{
	if (mIsEvent == false)
	{
		return;
	}
	mIsEvent = false;

	// Stop using the Connection events  
	HRESULT hResult;

	hResult = this->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&mConnectionPointContainer);

	if (FAILED(hResult))
		return;

	hResult = mConnectionPointContainer->FindConnectionPoint(__uuidof(ConnectionEvents), &mConnectionPoint);
	mConnectionPointContainer->Release();

	if (FAILED(hResult))
		return;

	hResult = mConnectionPoint->Unadvise(mConnEvt);
	mConnectionPoint->Release();

	if (FAILED(hResult))
		return;
}

const WCHAR* ADOConnection::GetDatabaseName() const
{
	auto connectionInterface = this->GetInterfacePtr();
	if (!connectionInterface)
	{
		return nullptr;
	}

	return connectionInterface->GetDefaultDatabase();
}

ADOConnectionInfo ADOConnection::GetConnectionInfo() const
{
	return mConnectionInfo;
}

void ADOConnection::SetConnectionInfo(const ADOConnectionInfo& info)
{
	mConnectionInfo = info;
}

_bstr_t ADOConnection::GetConnectionString()
{
	auto connectionInterface = this->GetInterfacePtr();
	if (!connectionInterface)
	{
		return _bstr_t();
	}

	return connectionInterface->GetConnectionString();
}

void ADOConnection::Initlialze()
{
	HRESULT hResult = S_FALSE;

	// CreateInstance
	hResult = this->CreateInstance(__uuidof(Connection));
	if (FAILED(hResult))
	{
		return;
	}

	auto connectionInterface = this->GetInterfacePtr();
	if (!connectionInterface)
	{
		return;
	}

	//DefaultSetting
	const long					connectionTimeout = 30;
	const long					commandTimeout = 30;
	const CursorLocationEnum	cursorLocation = CursorLocationEnum::adUseClient;
	const IsolationLevelEnum	isolationLevel = IsolationLevelEnum::adXactUnspecified;
	const ConnectModeEnum		connectMode = ConnectModeEnum::adModeUnknown;

	//connectionInterface->PutAttributes(0);
	connectionInterface->PutCommandTimeout(commandTimeout);
	//connectionInterface->PutConnectionString();
	connectionInterface->PutConnectionTimeout(connectionTimeout);
	connectionInterface->PutCursorLocation(cursorLocation);
	//connectionInterface->PutDefaultDatabase();
	//connectionInterface->PutIsolationLevel(isolationLevel);
	//connectionInterface->PutMode(connectMode);
	//connectionInterface->PutProvider();
}

void ADOConnection::UnInitlialze()
{
	
}
