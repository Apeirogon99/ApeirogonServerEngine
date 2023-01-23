#include "pch.h"
#include "EngineAssert.h"

EngineAssert::EngineAssert()
{
}

EngineAssert::~EngineAssert()
{
}

void EngineAssert::CRASH(int32 expr, LPCWSTR exprWStr, LPCWSTR file, int32 line)
{
	if (expr)
	{
		return;
	}

	std::wstring message;
	message += L"Assert : ";
	message += exprWStr;
	message += L", File : ";
	message += file;
	message += L", Line : ";
	message += std::to_wstring(line);

	wprintf(L"CRASH : %ws\n", message.c_str());
	mWinDump.ExceptionFilter(NULL);
}
