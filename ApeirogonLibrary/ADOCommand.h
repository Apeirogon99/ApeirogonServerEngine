#pragma once
class ADOCommand : public _CommandPtr
{
public:
	APEIROGON_API ADOCommand();
	APEIROGON_API ~ADOCommand();

private:
	ADOCommand& operator=(const ADOCommand&) = delete;
	ADOCommand(const ADOCommand&) = delete;

public:
	APEIROGON_API void			ResetStoredProcedure();
	APEIROGON_API void			SetStoredProcedure(ADOConnection& connection, const WCHAR* storedProcName);
	APEIROGON_API void			ExecuteStoredProcedure(ADORecordset& recordset, EExcuteReturnType type);

	APEIROGON_API void			SetInputParam(const WCHAR* inputName, ADOVariant& value);
	APEIROGON_API void			SetOutputParam(const WCHAR* outputName, DataTypeEnum inDataType, long inDataSize);
	APEIROGON_API void			SetOutputParam(const WCHAR* outputName, ADOVariant& value);
	APEIROGON_API void			SetReturnParam();

	APEIROGON_API void			SetParam(int32 index, const ADOVariant& value);

	APEIROGON_API int32			GetReturnParam();
	APEIROGON_API ADOVariant	GetParam(const int16 index);
	APEIROGON_API ADOVariant	GetParam(const WCHAR* name);
	APEIROGON_API ADOVariant	GetOutputParam(const WCHAR* name);

protected:
	void Initlialze();
	void UnInitlialze();
	HRESULT PutRefActiveConnection(ADOConnection& connection);

private:
	const WCHAR*	mParams[100] = { nullptr, };
	int32			mCurParamsCount = 0;
};

