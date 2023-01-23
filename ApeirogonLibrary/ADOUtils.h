#pragma once

enum class EDBMSTypes
{
	None,
	MSSQL,
	MYSQL,
	ORACLE
};

enum class EExcuteReturnType
{
	Not_Return,
	Wait_Return,
	Async_Return
};

class ADOConnectionInfo
{
public:
	//Security = SqlClinet, OleDb(SPPI), OracleClinet
	//Trusted = ODBC
	ADO_API ADOConnectionInfo();
	ADO_API ADOConnectionInfo(const WCHAR* provider, const WCHAR* server, const WCHAR* database, const WCHAR* security, const WCHAR* trusted, const WCHAR* id, const WCHAR* password, const EDBMSTypes type);

	ADO_API ADOConnectionInfo(const ADOConnectionInfo& info);

	ADO_API ADOConnectionInfo& operator=(const ADOConnectionInfo& info);

public:
	ADO_API void SetInfo(const WCHAR* provider, const WCHAR* server, const WCHAR* database, const WCHAR* security, const WCHAR* trusted, const WCHAR* id, const WCHAR* password);
	ADO_API const WCHAR* ToString() const;

public:
	//const WCHAR*	mProvider;
	//const WCHAR*	mServer;
	//const WCHAR*	mDatabase;
	//const WCHAR*	mSecurity;
	//const WCHAR*	mTrusted;
	//const WCHAR*	mId;
	//const WCHAR*	mPassword;
	EDBMSTypes mType;

private:
	WCHAR			mConnectString[128];
};

class ADOUtils
{
public:

	static DBTYPEENUM	ConvertToDBTYPE(const VARTYPE varType);
	static DataTypeEnum ConvertToDATATYPE(const DBTYPEENUM dbType);
	static DataTypeEnum VarType2DataType(const VARTYPE varType);

	static long			GetDataTypeSize(const _variant_t& value, DataTypeEnum dataType);

	static ADO_API void ADOError(const WCHAR* funcName, _com_error& error);
};

