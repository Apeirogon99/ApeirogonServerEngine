#include "pch.h"
#include "Runtime.h"

Runtime::Runtime(const WCHAR* func, ETime time) : mFunc(func), mTime(time), mStart(), mEnd()
{
	 
}

Runtime::~Runtime()
{
}

void Runtime::Start()
{
	//wprintf(L"[%ws::RuntimeStart]\n", mFunc);
	mStart = clock();
}

double Runtime::End()
{
	mEnd = clock();
	double result = static_cast<double>(mEnd - mStart);
	double divTime = static_cast<double>(mTime);
	double time = 0.0f;

	if (divTime == 0)
	{
		wprintf(L"[Runtime::End()] : dived time is 0\n");
	}

	switch (mTime)
	{
	case ETime::ms:
	case ETime::second:
	case ETime::minute:
		time = result / divTime;
		break;
	default:
		break;
	}

	//wprintf(L"[%ws::RuntimeEnd] = %lf(%ws)\n", mFunc, time, TimeToString());
	return time;
}

const WCHAR* Runtime::TimeToString()
{
	switch (mTime)
	{
	case ETime::ms:
		return L"ms";
		break;
	case ETime::second:
		return L"sec";
		break;
	case ETime::minute:
		return L"min";
		break;
	default:
		return L"ERROR";
		break;
	}
}
