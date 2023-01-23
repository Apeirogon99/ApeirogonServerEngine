#include "pch.h"
#include "SimpleTime.h"

Time::Time()
{
}

Time::~Time()
{
}

std::wstring Time::NowTime()
{
	struct tm		timeptr;
	wchar_t			dest[20];
	time_t			temp;
	std::wstring	curTime;

	temp = time(NULL);
	localtime_s(&timeptr, &temp);

	wcsftime(dest, sizeof(dest), L"[%Y.%m.%d-%H.%M.%S]", &timeptr);
	curTime += dest;

	return curTime;
}
