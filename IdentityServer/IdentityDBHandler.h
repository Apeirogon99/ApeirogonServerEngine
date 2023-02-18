#pragma once

enum class EQueryID
{

};

ESPResult SP_Singin(ADOVariant inId, ADOVariant inPassword, ADORecordset& outRecord);

class IdentityDBHandler
{
public:
	static void Init()
	{
		HRESULT hResult = S_FALSE;

		hResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if (FAILED(hResult))
		{
			return;
		}
	}

	static void ShutDown()
	{
		CoUninitialize();
	}

private:
	bool MakeQuery(ADOConnectionInfo inConnectInfo, const WCHAR* inStoredProcedure)
	{
		ADOConnection conn;
		conn.Open(inConnectInfo);

		ADOCommand cmd;
		cmd.SetStoredProcedure(conn, inStoredProcedure);
		cmd.SetReturnParam();
	}
};

