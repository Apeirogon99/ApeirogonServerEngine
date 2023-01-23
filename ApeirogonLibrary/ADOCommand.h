#pragma once
class ADOCommand : public _CommandPtr
{
public:
	ADO_API ADOCommand();
	ADO_API ~ADOCommand();

private:
	ADOCommand& operator=(const ADOCommand&) = delete;
	ADOCommand(const ADOCommand&) = delete;

public:
	ADO_API void		SetStoredProcedure(ADOConnection& connection, const WCHAR* storedProcName);
	ADO_API void		ExecuteStoredProcedure(ADORecordset& recordset, EExcuteReturnType type);

	ADO_API void		SetInputParam(const WCHAR* inputName, ADOVariant& value);
	ADO_API void		SetOutputParam(const WCHAR* outputName, ADOVariant& value);
	ADO_API void		SetReturnParam();

	ADO_API void		SetParam(int32 index, const ADOVariant& value);

	ADO_API int32		GetReturnParam();
	ADO_API ADOVariant	GetParam(const int16 index);
	ADO_API ADOVariant	GetParam(const WCHAR* name);

protected:
	void Initlialze();
	void UnInitlialze();
	HRESULT PutRefActiveConnection(ADOConnection& connection);
};

