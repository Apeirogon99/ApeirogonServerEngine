#include "pch.h"
#include "SimpleTime.h"

Time::Time()
{
}

Time::~Time()
{
}

std::wstring Time::NowTime(const bool useMs)
{
	struct tm		timeptr;
	wchar_t			now[100];
	time_t			temp;
	std::wstring	curTime;

	temp = time(NULL);
	localtime_s(&timeptr, &temp);

	if (useMs)
	{
		struct timeb tb;
		ftime(&tb);

		wsprintf(now, L"[%02d.%02d.%02d-%02d.%02d.%02d:%03d]", timeptr.tm_year + 1900, timeptr.tm_mon + 1, timeptr.tm_mday, timeptr.tm_hour, timeptr.tm_min, timeptr.tm_sec, tb.millitm);
	}
	else
	{
		wsprintf(now, L"[%02d.%02d.%02d-%02d.%02d.%02d]", timeptr.tm_year + 1900, timeptr.tm_mon + 1, timeptr.tm_mday, timeptr.tm_hour, timeptr.tm_min, timeptr.tm_sec);
	}

	curTime = now;

	return curTime;
}

int64 Time::GetUTCTime()
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	//std::chrono::utc_clock::time_point now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::utc_clock::now());
	std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
	return milliseconds.count();
}
