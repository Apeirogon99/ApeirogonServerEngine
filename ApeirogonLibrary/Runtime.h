#pragma once

#ifdef DLL_DEVELOPMENT
#define TIME_API __declspec(dllimport)
#else
#define TIME_API __declspec(dllexport)
#endif // DLL_DEVELOPMENT

enum class ETime
{
	ms = 1,
	second = 1000,
	minute = 10000,
};

class Runtime
{
public:
	TIME_API Runtime(const WCHAR* func, ETime time = ETime::ms);
	TIME_API ~Runtime();

	TIME_API void Start();
	TIME_API double End();

protected:
	const WCHAR* TimeToString();

private:
	const WCHAR*	mFunc;
	clock_t			mStart;
	clock_t			mEnd;
	ETime			mTime;
};

