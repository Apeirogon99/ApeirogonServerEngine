#include "pch.h"

std::wstring DBError::GetDBErrorToString(ESPResult result)
{
	switch (result)
	{
	case ESPResult::Success:
		return L"성공적으로 SP가 수행됨";
		break;
	case ESPResult::Failed:
		return L"SP가 실패하였습니다";
		break;
	case ESPResult::Close:
		return L"Record가 닫혀 있습니다";
		break;
	case ESPResult::Index_None:
		return L"원하는 값을 찾을 수 없습니다";
		break;
	default:
		return L"알 수 없는 오류입니다";
		break;
	}
    return  L"알 수 없는 오류입니다";
}
