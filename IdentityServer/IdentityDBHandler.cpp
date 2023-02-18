#include "pch.h"
#include "IdentityDBHandler.h"

ESPResult SP_Singin(ADOVariant inId, ADOVariant inPassword, ADORecordset& outRecord)
{
	ADOConnectionInfo ConnectionInfo(L"SQLOLEDB", L"APEIROGON", L"IdentityDB", L"SSPI", L"NO", L"apeirogon", L"1248", EDBMSTypes::MSSQL);
	ADOConnection conn;
	conn.Open(ConnectionInfo);

	ADOCommand cmd;
	cmd.SetStoredProcedure(conn, L"dbo.singin");
	cmd.SetReturnParam();
	cmd.SetInputParam(L"@id", inId);
	cmd.SetInputParam(L"@password", inPassword);

	ADORecordset rs;
	cmd.ExecuteStoredProcedure(outRecord, EExcuteReturnType::Wait_Return);

	outRecord = rs.GetInterfacePtr();

	int32 ret = cmd.GetReturnParam();
	if (ret == 0)
	{
		return ESPResult::Failed;
	}

	if (false == outRecord.IsOpen())
	{
		return ESPResult::Close;
	}

	return ESPResult::Success;
}
