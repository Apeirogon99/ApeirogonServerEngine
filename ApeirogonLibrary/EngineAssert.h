#pragma once

#undef ASSERT
#define ASSERT(expr) EngineAssert::CRASH(expr, _T(#expr), _T(__FILE__), __LINE__)

class EngineAssert
{
public:
	EngineAssert();
	~EngineAssert();

public:
	APEIROGON_API static void CRASH(int32 expr, LPCWSTR exprWStr, LPCWSTR file, int32 line);

private:
	static WinDump mWinDump;
};

