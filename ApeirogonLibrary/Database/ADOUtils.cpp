#include "pch.h"
#include "ADOUtils.h"

ADOConnectionInfo::ADOConnectionInfo() : mType(EDBMSTypes::None)
{
	wmemset(mConnectString, 0, Default::ConnectionSize);
}

ADOConnectionInfo::ADOConnectionInfo(const WCHAR* provider, const WCHAR* server, const WCHAR* database, const WCHAR* security, const WCHAR* trusted, const WCHAR* id, const WCHAR* password, const EDBMSTypes type) : mType(type)
{
    wmemset(mConnectString, 0, Default::ConnectionSize);
	switch (type)
	{
	case EDBMSTypes::None:
		break;
	case EDBMSTypes::MSSQL:
		swprintf(mConnectString, Default::ConnectionSize, L"Provider=%ws;Data Source=%ws;Initial Catalog=%ws;Integrated Security=%ws;Userid=%ws;password=%ws;", provider, server, database, security, id, password);
		break;
	case EDBMSTypes::AWS_MSSQL:
		swprintf(mConnectString, Default::ConnectionSize, L"Provider=%ws;Data Source=%ws;Initial Catalog=%ws;User ID=%ws;Password=%ws;", provider, server, database, id, password);
		break;
	case EDBMSTypes::MYSQL:
		break;
	case EDBMSTypes::ORACLE:
		break;
	default:
		break;
	}
}

ADOConnectionInfo::ADOConnectionInfo(const ADOConnectionInfo& info)
{
    wmemset(mConnectString, 0, Default::ConnectionSize);
    const WCHAR* connectString = info.ToString();
    const size_t strSize = wcslen(connectString);
    wmemcpy(mConnectString, connectString, strSize);

	mType = info.mType;
}

ADOConnectionInfo& ADOConnectionInfo::operator=(const ADOConnectionInfo& info)
{
	wmemset(mConnectString, 0, Default::ConnectionSize);
	const WCHAR* connectString = info.ToString();
	const size_t strSize = wcslen(connectString);
	wmemcpy(mConnectString, connectString, strSize);

	mType = info.mType;

	return *this;
}

void ADOConnectionInfo::SetInfo(const WCHAR* provider, const WCHAR* server, const WCHAR* database, const WCHAR* security, const WCHAR* trusted, const WCHAR* id, const WCHAR* password)
{
	wmemset(mConnectString, 0, Default::ConnectionSize);
	swprintf(mConnectString, Default::ConnectionSize, L"Provider=%ws;Data Source=%ws;Initial Catalog=%ws;Integrated Security=%ws;Userid=%ws;password=%ws;", provider, server, database, security, id, password);
}

const WCHAR* ADOConnectionInfo::ToString() const
{
    return (wcscmp(mConnectString, L"") == 0) ? nullptr : mConnectString;
}

DBTYPEENUM ADOUtils::ConvertToDBTYPE(const VARTYPE varType)
{
	switch (varType)
	{
	case VARENUM::VT_I8:				return DBTYPEENUM::DBTYPE_I8;
	case VARENUM::VT_BLOB:				return DBTYPEENUM::DBTYPE_BYTES;
	case VARENUM::VT_BOOL:				return DBTYPEENUM::DBTYPE_BOOL;
	case VARENUM::VT_BSTR:				return DBTYPEENUM::DBTYPE_BSTR;
		//case VARENUM::VT_:				return static_cast<DBTYPEENUM>(DBTYPEENUM15::DBTYPE_HCHAPTER);		//타입에 없음
	case VARENUM::VT_LPSTR:				return DBTYPEENUM::DBTYPE_STR;
	case VARENUM::VT_CY:				return DBTYPEENUM::DBTYPE_CY;
	case VARENUM::VT_DATE:				return DBTYPEENUM::DBTYPE_DATE;
		//case VARENUM::VT_DATE:			return DBTYPEENUM::DBTYPE_DBDATE;
		//case VARENUM::VT_TIME:			return DBTYPEENUM::DBTYPE_DBTIME;
		//case VARENUM::VT_BOOL:			return DBTYPEENUM::DBTYPE_DBTIMESTAMP;
	case VARENUM::VT_DECIMAL:			return DBTYPEENUM::DBTYPE_DECIMAL;
	case VARENUM::VT_R8:				return DBTYPEENUM::DBTYPE_R8;
	case VARENUM::VT_ERROR:				return DBTYPEENUM::DBTYPE_ERROR;
	case VARENUM::VT_FILETIME:			return static_cast<DBTYPEENUM>(DBTYPEENUM20::DBTYPE_FILETIME);
	case VARENUM::VT_VERSIONED_STREAM:	return DBTYPEENUM::DBTYPE_GUID;
	case VARENUM::VT_DISPATCH:			return DBTYPEENUM::DBTYPE_IDISPATCH;
	case VARENUM::VT_I4:
	case VARENUM::VT_INT:				return DBTYPEENUM::DBTYPE_I4;			//VARENUM에 I4있긴한데 INT로 인식함
	case VARENUM::VT_UNKNOWN:			return DBTYPEENUM::DBTYPE_IUNKNOWN;
		//case VARENUM::VT_NUM:				return DBTYPEENUM::DBTYPE_NUMERIC;
		//case VARENUM::VT_PRO:				return static_cast<DBTYPEENUM>(DBTYPEENUM20::DBTYPE_PROPVARIANT);
	case VARENUM::VT_R4:				return DBTYPEENUM::DBTYPE_R4;
	case VARENUM::VT_I2:				return DBTYPEENUM::DBTYPE_I2;
	case VARENUM::VT_I1:				return DBTYPEENUM::DBTYPE_I1;
	case VARENUM::VT_UI8:				return DBTYPEENUM::DBTYPE_UI8;
	case VARENUM::VT_UI4:				return DBTYPEENUM::DBTYPE_UI4;
	case VARENUM::VT_UI2:				return DBTYPEENUM::DBTYPE_UI2;
	case VARENUM::VT_UI1:				return DBTYPEENUM::DBTYPE_UI1;
	case VARENUM::VT_VARIANT:				return DBTYPEENUM::DBTYPE_VARIANT;
	case VARENUM::VT_LPWSTR:			return DBTYPEENUM::DBTYPE_WSTR;

	default:
		wprintf(L"[OLEDBTypeMappings] DefaultType : %d\n", varType);
		return DBTYPEENUM::DBTYPE_EMPTY;
	}

	return DBTYPEENUM::DBTYPE_EMPTY;
}

DataTypeEnum ADOUtils::ConvertToDATATYPE(const DBTYPEENUM dbType)
{
	switch (dbType)
	{
	case DBTYPEENUM::DBTYPE_I8:				return DataTypeEnum::adBigInt;
	case DBTYPEENUM::DBTYPE_BYTES:			return DataTypeEnum::adBinary;
	case DBTYPEENUM::DBTYPE_BOOL:			return DataTypeEnum::adBoolean;
	case DBTYPEENUM::DBTYPE_BSTR:			return DataTypeEnum::adBSTR;
	case DBTYPEENUM15::DBTYPE_HCHAPTER:		return DataTypeEnum::adChapter;
	case DBTYPEENUM::DBTYPE_STR:			return DataTypeEnum::adChar;
	case DBTYPEENUM::DBTYPE_CY:				return DataTypeEnum::adCurrency;
	case DBTYPEENUM::DBTYPE_DATE:			return DataTypeEnum::adDate;
	case DBTYPEENUM::DBTYPE_DBDATE:			return DataTypeEnum::adDBDate;
	case DBTYPEENUM::DBTYPE_DBTIME:			return DataTypeEnum::adDBTime;
	case DBTYPEENUM::DBTYPE_DBTIMESTAMP:	return DataTypeEnum::adDBTimeStamp;
	case DBTYPEENUM::DBTYPE_DECIMAL:		return DataTypeEnum::adDecimal;
	case DBTYPEENUM::DBTYPE_R8:				return DataTypeEnum::adDouble;
	case DBTYPEENUM::DBTYPE_ERROR:			return DataTypeEnum::adError;
	case DBTYPEENUM20::DBTYPE_FILETIME:		return DataTypeEnum::adFileTime;
	case DBTYPEENUM::DBTYPE_GUID:			return DataTypeEnum::adGUID;
	case DBTYPEENUM::DBTYPE_IDISPATCH:		return DataTypeEnum::adIDispatch;
	case DBTYPEENUM::DBTYPE_I4:				return DataTypeEnum::adInteger;
	case DBTYPEENUM::DBTYPE_IUNKNOWN:		return DataTypeEnum::adIUnknown;
	case DBTYPEENUM::DBTYPE_NUMERIC:		return DataTypeEnum::adNumeric;
	case DBTYPEENUM20::DBTYPE_PROPVARIANT:	return DataTypeEnum::adPropVariant;
	case DBTYPEENUM::DBTYPE_R4:				return DataTypeEnum::adSingle;
	case DBTYPEENUM::DBTYPE_I2:				return DataTypeEnum::adSmallInt;
	case DBTYPEENUM::DBTYPE_I1:				return DataTypeEnum::adTinyInt;
	case DBTYPEENUM::DBTYPE_UI8:			return DataTypeEnum::adUnsignedBigInt;
	case DBTYPEENUM::DBTYPE_UI4:			return DataTypeEnum::adUnsignedInt;
	case DBTYPEENUM::DBTYPE_UI2:			return DataTypeEnum::adUnsignedSmallInt;
	case DBTYPEENUM::DBTYPE_UI1:			return DataTypeEnum::adUnsignedTinyInt;
	case DBTYPEENUM::DBTYPE_VARIANT:		return DataTypeEnum::adVariant;
	case DBTYPEENUM::DBTYPE_WSTR:			return DataTypeEnum::adWChar;
		//case DBTYPEENUM::DBTYPE_UDT:			return DataTypeEnum::adUserDefined;	지원X
		//case DBTYPEENUM::DBTYPE_VARNUMERIC:	return DataTypeEnum::adVarNumeric;	지원X
	default:
		wprintf(L"[ADOTypeMappings] DefaultType : %d\n", dbType);
		return DataTypeEnum::adEmpty;
	}

	return DataTypeEnum::adEmpty;
}


DataTypeEnum ADOUtils::VarType2DataType(const VARTYPE varType)
{
	DBTYPEENUM		dbtype = ConvertToDBTYPE(varType);
	DataTypeEnum	datatype = ConvertToDATATYPE(dbtype);
	return datatype;
}

long ADOUtils::GetDataTypeSize(const _variant_t& value, DataTypeEnum dataType)
{
	switch (dataType)
	{
	case DataTypeEnum::adBigInt:			return sizeof(LONGLONG);
	case DataTypeEnum::adBinary:			return sizeof(BYTE);
	case DataTypeEnum::adBoolean:			return sizeof(BOOLEAN);
	case DataTypeEnum::adBSTR:				return static_cast<long>(wcslen(value.bstrVal));
	case DataTypeEnum::adChapter:			return sizeof(HCHAPTER);
	case DataTypeEnum::adChar:				return static_cast<long>(wcslen(value.bstrVal));	//문자열의 값
	case DataTypeEnum::adCurrency:			return sizeof(CY);
	case DataTypeEnum::adDate:				return sizeof(DATE);
	case DataTypeEnum::adDBDate:			return sizeof(DBDATE);
	case DataTypeEnum::adDBTime:			return sizeof(DBTIME);
	case DataTypeEnum::adDBTimeStamp:		return sizeof(DBTIMESTAMP);
	case DataTypeEnum::adDecimal:			return sizeof(DECIMAL);
	case DataTypeEnum::adDouble:			return sizeof(DOUBLE);
	case DataTypeEnum::adError:				return sizeof(int32);								//32비트 오류 코드
	case DataTypeEnum::adFileTime:			return sizeof(FILETIME);
	case DataTypeEnum::adGUID:				return sizeof(GUID);
	case DataTypeEnum::adIDispatch:			return sizeof(IDispatch);
	case DataTypeEnum::adInteger:			return sizeof(INT);
	//case DataTypeEnum::adIUnknown:		return sizeof(IUnknown);							//ADO 에서 지원하지 않음
	case DataTypeEnum::adNumeric:			return sizeof(DECIMAL);								//고정된 정밀도와 배율이 있는 숫자
	case DataTypeEnum::adPropVariant:		return sizeof(PROPVARIANT);
	case DataTypeEnum::adSingle:			return sizeof(FLOAT);								//단정밀도 부동 소수점 값
	case DataTypeEnum::adSmallInt:			return sizeof(SHORT);
	case DataTypeEnum::adTinyInt:			return sizeof(CHAR);								//1바이트 부호 있는 정수
	case DataTypeEnum::adUnsignedBigInt:	return sizeof(ULONGLONG);
	case DataTypeEnum::adUnsignedInt:		return sizeof(UINT);
	case DataTypeEnum::adUnsignedSmallInt:	return sizeof(USHORT);
	case DataTypeEnum::adUnsignedTinyInt:	return sizeof(UCHAR);
	//case DataTypeEnum::adVariant:			return sizeof(VARIANT);								//ADO에서 지원하지 않음
	case DataTypeEnum::adWChar:				return static_cast<long>(wcslen(value.bstrVal));	//내부 보니까 안에 VT_BSTR로 바뀜
	//case DataTypeEnum::adUserDefined;	지원X
	//case DataTypeEnum::adVarNumeric;	지원X
	default:
		wprintf(L"[GetDataTypeSize] DefaultType : %d\n", dataType);
		return -1;
	}

	return -1;
}

void ADOUtils::ADOError(const WCHAR* funcName, _com_error& error)
{
	_bstr_t bstrDescription(error.Description());
	wprintf(L"[ADOError][%ws] : %ws\n", funcName, bstrDescription.GetBSTR());
}
