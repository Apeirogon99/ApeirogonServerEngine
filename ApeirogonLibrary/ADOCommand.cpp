#include "pch.h"
#include "ADOCommand.h"

ADOCommand::ADOCommand()
{
	Initlialze();
}

ADOCommand::~ADOCommand()
{
	UnInitlialze();
}

void ADOCommand::Initlialze()
{
	HRESULT hResult = S_FALSE;

	hResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hResult))
	{
		return;
	}

	// CreateInstance
	hResult = this->CreateInstance(__uuidof(Command));
	if (FAILED(hResult))
	{
		return;
	}
	
	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		return;
	}

	VARIANT_BOOL prepared = VARIANT_TRUE;
	long timeout = 30;
	CommandTypeEnum commandType = CommandTypeEnum::adCmdStoredProc;

	commandInterface->PutPrepared(prepared);
	commandInterface->PutCommandTimeout(timeout);
	commandInterface->PutCommandType(commandType);
}

void ADOCommand::UnInitlialze()
{
	CoUninitialize();
}

HRESULT ADOCommand::PutRefActiveConnection(ADOConnection& connection)
{
	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		return S_FALSE;
	}

	if (!connection)
	{
		return S_FALSE;
	}

	commandInterface->PutRefActiveConnection(connection);

	return commandInterface->GetActiveConnection() != nullptr ? S_OK : S_FALSE;
}

void ADOCommand::SetStoredProcedure(ADOConnection& connection, const WCHAR* storedProcName)
{
	HRESULT hResult = S_FALSE;

	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		wprintf(L"[DBCommand::StoredProcCommand] is not valid commandInterface\n");
		return;
	}

	//commandInterface->ActiveConnection = connection;
	hResult = PutRefActiveConnection(connection);
	if (FAILED(hResult))
	{
		wprintf(L"[DBCommand::StoredProcCommand] Failed to PutRefActiveConnection\n");
		return;
	}

	commandInterface->PutCommandText(storedProcName);
}

void ADOCommand::ExecuteStoredProcedure(ADORecordset& recordset, EExcuteReturnType type)
{
	HRESULT hResult = S_FALSE;

	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		return;
	}

	if (!recordset)
	{
		return;
	}

	recordset->PutRefSource(*this);

	long executeOptions;
	switch (type)
	{
	case EExcuteReturnType::Not_Return:
		executeOptions = adCmdStoredProc | adExecuteNoRecords;
		recordset = commandInterface->Execute(NULL, &vtMissing, executeOptions);
		break;
	case EExcuteReturnType::Wait_Return:
		executeOptions = adCmdStoredProc | adAsyncFetchNonBlocking;
		recordset = commandInterface->Execute(NULL, &vtMissing, executeOptions);
		break;
	case EExcuteReturnType::Async_Return:
		executeOptions = adCmdStoredProc | adAsyncExecute;
		recordset = commandInterface->Execute(NULL, &vtMissing, executeOptions);
		break;
	default:
		wprintf(L"[DBCommand::ExecuteStoredProcedure] is exits excute type\n");
		return;
		break;
	}
}

void ADOCommand::SetInputParam(const WCHAR* inputName, ADOVariant& value)
{
	HRESULT					result = S_FALSE;
	DataTypeEnum			dataType = ADOUtils::VarType2DataType(value.mVar.vt);
	long					dataTypeSize = ADOUtils::GetDataTypeSize(value.mVar, dataType);
	ParameterDirectionEnum	parmDirction = ParameterDirectionEnum::adParamInput;

	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		return;
	}

	_ParameterPtr paramRet = commandInterface->CreateParameter(inputName, dataType, parmDirction, dataTypeSize, value.mVar);
	if (!paramRet)
	{
		return;
	}

	result = commandInterface->Parameters->Append(paramRet);

	paramRet->Value = value.mVar;

	if (FAILED(result))
	{
		return;
	}
}

void ADOCommand::SetOutputParam(const WCHAR* outputName, ADOVariant& value)
{
	HRESULT					result = S_FALSE;
	DataTypeEnum			dataType = ADOUtils::VarType2DataType(value.mVar.vt);
	long					dataTypeSize = ADOUtils::GetDataTypeSize(value.mVar, dataType);
	ParameterDirectionEnum	parmDirction = ParameterDirectionEnum::adParamOutput;

	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		return;
	}

	_ParameterPtr paramRet = commandInterface->CreateParameter(outputName, dataType, parmDirction, dataTypeSize, value.mVar);

	if (!paramRet)
	{
		return;
	}

	result = commandInterface->Parameters->Append(paramRet);

	paramRet->Value = value.mVar;

	if (FAILED(result))
	{
		return;
	}
}

void ADOCommand::SetReturnParam()
{
	HRESULT					result = S_FALSE;
	DataTypeEnum			returnType = DataTypeEnum::adInteger;
	ParameterDirectionEnum	parmDirction = ParameterDirectionEnum::adParamReturnValue;
	long					paramsize = sizeof(int32);

	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		return;
	}

	_ParameterPtr paramRet = commandInterface->CreateParameter("return", returnType, parmDirction, paramsize);

	if (!paramRet)
	{
		return;
	}

	result = commandInterface->Parameters->Append(paramRet);

	if (FAILED(result))
	{
		return;
	}
}

void ADOCommand::SetParam(int32 index, const ADOVariant& value)
{
	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		return;
	}

	auto paramsInterface = commandInterface->GetParameters();
	if (!paramsInterface)
	{
		wprintf(L"[SetParam] is not valid params\n");
		return;
	}

	paramsInterface->Item[index]->PutValue(value.mVar);
}

int32 ADOCommand::GetReturnParam()
{
	_variant_t returnIndex;
	returnIndex.vt = VT_I2;
	returnIndex.iVal = 0;

	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		wprintf(L"[DBCommand::GetReturnParam] is not valid commandInterface\n");
		return NULL;
	}

	auto parameters = commandInterface->GetParameters();
	if (!parameters)
	{
		wprintf(L"[DBCommand::GetReturnParam] is not valid parameters\n");
		return NULL;
	}

	int32 tempReturnParam = parameters->GetItem(returnIndex)->GetValue();
	return tempReturnParam;
}

ADOVariant ADOCommand::GetParam(const int16 index)
{
	_variant_t ParamIndex;
	ParamIndex.vt = VT_I2;
	ParamIndex.iVal = index;

	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		wprintf(L"[DBCommand::GetParam, index] is not valid commandInterface\n");
		return NULL;
	}

	auto parameters = commandInterface->GetParameters();
	if (!parameters)
	{
		wprintf(L"[DBCommand::GetParam, index] is not valid parameters\n");
		return NULL;
	}

	_variant_t tempParam = parameters->GetItem(ParamIndex)->GetValue();
	return tempParam;
}

ADOVariant ADOCommand::GetParam(const WCHAR* name)
{
	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		wprintf(L"[DBCommand::GetParam, name] is not valid commandInterface\n");
		return NULL;
	}

	auto parameters = commandInterface->GetParameters();
	if (!parameters)
	{
		wprintf(L"[DBCommand::GetParam, name] is not valid parameters\n");
		return NULL;
	}

	_variant_t tempParam = parameters->Item[name]->GetValue();
	return tempParam;
}