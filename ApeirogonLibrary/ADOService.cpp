#include "pch.h"
#include "ADOService.h"

using namespace std;

ADOService::ADOService(const size_t poolSize) : mPoolSize(poolSize)
{
	mConnections.reserve(mPoolSize);
	StartADO();
}

ADOService::~ADOService()
{
	CloseAllConnection();
	EndADO();
}

bool ADOService::Run()
{
	while (mIsService)
	{
		KeepConnection();
	}

	return false;
}

void ADOService::PushConnectionPool(ADOConnection& conn, const ADOConnectionInfo& info)
{
	conn.Open(info);
	mConnections.push_back(conn);
}

void ADOService::PrintConnectionPoolState()
{
	if (mConnections.empty())
	{
		return;
	}

	wprintf(L"\nPrintConnectionPoolState\n");
	for (ADOConnection& connection : mConnections)
	{
		wprintf(L"[Database::%ws] - ", connection.GetDatabaseName());
		if (connection.IsOpen())
		{
			wprintf(L"open\n");
		}
		else
		{
			wprintf(L"close\n");
		}
	}
	wprintf(L"\n");
}

bool ADOService::WaitConnectionPool()
{
	int32 IsReady = 0;

	if (mConnections.empty())
	{
		return false;
	}

	for (ADOConnection& connection : mConnections)
	{
		if (connection.IsOpen())
		{
			IsReady++;
		}
	}

	return (IsReady - mConnections.size() == 0) ? true : false;
}

void ADOService::KeepConnection()
{
	if (mConnections.empty())
	{
		return;
	}

	for (ADOConnection& connection : mConnections)
	{
		if (!connection.IsOpen() || connection->GetState() != adStateConnecting)
		{
			ADOConnectionInfo info = connection.GetConnectionInfo();
			connection.OpenEx(info);
		}
	}
}

void ADOService::CloseAllConnection()
{
	for (ADOConnection& connection : mConnections)
	{
		connection.Close();
	}
}

void ADOService::StartADO()
{
	HRESULT hResult = S_FALSE;

	hResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hResult))
	{
		return;
	}

	mIsService = true;

}

void ADOService::EndADO()
{
	CoUninitialize();
}
