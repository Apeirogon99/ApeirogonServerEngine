#pragma once

class TimeStamp
{
public:
	APEIROGON_API TimeStamp(const WCHAR* inName);
	APEIROGON_API ~TimeStamp();

private:
	TimeStamp(TimeStamp&&) = delete;
	TimeStamp(const TimeStamp&) = delete;

	TimeStamp& operator=(TimeStamp&&) = delete;
	TimeStamp& operator=(const TimeStamp&) = delete;

public:
	APEIROGON_API void		StartTimeStamp();
	APEIROGON_API int64		GetTimeStamp();

protected:
	int64					GetEndTime(const ETime inTimeType = ETime::ms);

private:
	const WCHAR*							mName;
	std::chrono::steady_clock::time_point	mBeginClock;
	FastSpinLock							mLock;
};

