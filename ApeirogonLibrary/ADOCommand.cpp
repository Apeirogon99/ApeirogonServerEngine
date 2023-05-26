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

ADOCommand::ADOCommand(_CommandPtr inCommand)
{
	if (inCommand)
	{
		Initlialze();
		Attach(inCommand, true);
	}
}

ADOCommand& ADOCommand::operator=(_CommandPtr inCommand)
{
	if (inCommand)
	{
		Initlialze();
		Attach(inCommand, true);
	}

	return *this;
}

ADOCommand::ADOCommand(const ADOCommand& inCommand)
{
	if (inCommand)
	{
		Initlialze();

		mCurParamsCount = inCommand.mCurParamsCount;
		for (int32 param = 0; param < mCurParamsCount; ++param)
		{
			mParams[param] = inCommand.mParams[param];
		}

		mConnectionInfo = inCommand.mConnectionInfo;

		Attach(inCommand, true);
	}
}

ADOCommand& ADOCommand::operator=(const ADOCommand& inCommand)
{
	if (inCommand)
	{
		Initlialze();

		mCurParamsCount = inCommand.mCurParamsCount;
		for (int32 param = 0; param < mCurParamsCount; ++param)
		{
			mParams[param] = inCommand.mParams[param];
		}

		mConnectionInfo = inCommand.mConnectionInfo;

		Attach(inCommand, true);
	}

	return *this;
}

void ADOCommand::Initlialze()
{
	HRESULT hResult = S_FALSE;

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
	long timeout = 0;
	CommandTypeEnum commandType = CommandTypeEnum::adCmdStoredProc;

	commandInterface->PutPrepared(prepared);
	commandInterface->PutCommandTimeout(timeout);
	commandInterface->PutCommandType(commandType);
}

void ADOCommand::UnInitlialze()
{

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

void ADOCommand::ResetStoredProcedure()
{
	HRESULT hResult = S_FALSE;

	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		wprintf(L"[DBCommand::StoredProcCommand] is not valid commandInterface\n");
		return;
	}

	commandInterface->PutCommandText("");
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
	mConnectionInfo = connection.GetConnectionInfo();
	hResult = PutRefActiveConnection(connection);
	if (hResult == S_FALSE)
	{
		wprintf(L"[DBCommand::StoredProcCommand] Failed to PutRefActiveConnection\n");
		return;
	}
	this->GetInterfacePtr()->GetActiveConnection();
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
	recordset.SetConnectionInfo(mConnectionInfo);

	long executeOptions;
	switch (type)
	{
	case EExcuteReturnType::Not_Return:
		executeOptions = adCmdStoredProc | adExecuteNoRecords;
		recordset = commandInterface->Execute(NULL, &vtMissing, executeOptions);
		break;
	case EExcuteReturnType::Wait_Return:
		executeOptions = adCmdStoredProc | adExecuteRecord;
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

	mParams[mCurParamsCount++] = inputName;
}

void ADOCommand::SetOutputParam(const WCHAR* outputName, DataTypeEnum inDataType, long inDataSize)
{
	HRESULT					result = S_FALSE;
	ParameterDirectionEnum	parmDirction = ParameterDirectionEnum::adParamOutput;

	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		return;
	}

	_ParameterPtr paramRet = commandInterface->CreateParameter(outputName, inDataType, parmDirction, inDataSize, NULL);

	if (!paramRet)
	{
		return;
	}

	result = commandInterface->Parameters->Append(paramRet);

	//paramRet->Value = NULL;

	if (FAILED(result))
	{
		return;
	}

	mParams[mCurParamsCount++] = outputName;
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

	_ParameterPtr paramRet = commandInterface->CreateParameter(outputName, dataType, parmDirction, dataTypeSize, NULL);

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

	mParams[mCurParamsCount++] = outputName;
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

ADOVariant ADOCommand::GetOutputParam(const WCHAR* name)
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

	for (int i = 0; i < mCurParamsCount; ++i)
	{
		if (::wcscmp(mParams[i], name) == 0)
		{
			return GetParam(i + 1);
		}
	}

	_variant_t variant;
	return variant;
}

_variant_t ADOCommand::GetActiveConnetion()
{
	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		return S_FALSE;
	}

	auto connection = commandInterface->GetActiveConnection();
	if (!connection)
	{
		return _variant_t(NULL);
	}

	return _variant_t((IDispatch*)connection, true);
}

ADOVariant ADOCommand::GetActiveConnectionString()
{
	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		wprintf(L"[DBCommand::GetActiveConnection] is not valid commandInterface\n");
		return NULL;
	}

	ADOVariant connectionString = mConnectionInfo.ToString();

	return connectionString;
}

ADOVariant ADOCommand::GetCommandSource()
{
	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		wprintf(L"[DBCommand::GetCommandSource] is not valid commandInterface\n");
		return NULL;
	}

	ADOVariant comandSource = commandInterface->GetCommandText().GetBSTR();

	return comandSource;
}

bool ADOCommand::IsExecuteComplete() const
{
	auto commandInterface = this->GetInterfacePtr();
	if (!commandInterface)
	{
		wprintf(L"[DBCommand::IsExecuteComplete] is not valid commandInterface\n");
		return false;
	}

	long state = commandInterface->GetState();
	if (state == ObjectStateEnum::adStateClosed)
	{
		wprintf(L"[DBCommand::IsExecuteComplete] command is complete\n");
	}
	else
	{
		wprintf(L"[DBCommand::IsExecuteComplete] command is executing\n");
	}

	return (state == ObjectStateEnum::adStateClosed) ? true : false;
}
