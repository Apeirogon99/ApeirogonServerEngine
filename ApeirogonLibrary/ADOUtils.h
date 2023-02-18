#pragma once

#ifdef DLL_DEVELOPMENT
#define APEIROGON_API __declspec(dllimport)
#else
#define APEIROGON_API __declspec(dllexport)
#endif // DLL_DEVELOPMENT

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
	APEIROGON_API ADOConnectionInfo();
	APEIROGON_API ADOConnectionInfo(const WCHAR* provider, const WCHAR* server, const WCHAR* database, const WCHAR* security, const WCHAR* trusted, const WCHAR* id, const WCHAR* password, const EDBMSTypes type);

	APEIROGON_API ADOConnectionInfo(const ADOConnectionInfo& info);

	APEIROGON_API ADOConnectionInfo& operator=(const ADOConnectionInfo& info);

public:
	APEIROGON_API void SetInfo(const WCHAR* provider, const WCHAR* server, const WCHAR* database, const WCHAR* security, const WCHAR* trusted, const WCHAR* id, const WCHAR* password);
	APEIROGON_API const WCHAR* ToString() const;

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

	static APEIROGON_API void ADOError(const WCHAR* funcName, _com_error& error);
};

