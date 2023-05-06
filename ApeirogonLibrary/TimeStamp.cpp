#include "pch.h"
#include "TimeStamp.h"

using namespace std::chrono;
using namespace std;

TimeStamp::TimeStamp(const WCHAR* inName) : mName(inName), mBeginClock()
{
}

TimeStamp::~TimeStamp()
{
}

void TimeStamp::StartTimeStamp()
{
	mBeginClock = steady_clock::now();
}

int64 TimeStamp::GetTimeStamp()
{
	const int64 timeStamp = GetEndTime();
	return timeStamp;
}

int64 TimeStamp::GetEndTime(const ETime inTimeType)
{
	FastLockGuard lockGuard(mLock);
	std::chrono::steady_clock::time_point endClock = steady_clock::now();
	long long duration = duration_cast<milliseconds>(endClock - mBeginClock).count();
	return static_cast<int64>(duration);
}
