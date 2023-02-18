#pragma once

#ifdef DLL_DEVELOPMENT
#define APEIROGON_API __declspec(dllimport)
#else
#define APEIROGON_API __declspec(dllexport)
#endif // DLL_DEVELOPMENT

class FastSpinLock
{
	enum class Default
	{
		MAX_SPIN_COUNT = 0x400,
		LOCK_IS_TAKEN = 1,
		LOCK_IS_FREE = 0,
	};

public:
	APEIROGON_API FastSpinLock();
	APEIROGON_API ~FastSpinLock();

	FastSpinLock(const FastSpinLock& lock) = delete;
	FastSpinLock& operator=(const FastSpinLock& lock) = delete;

public:
	APEIROGON_API void Lock();
	APEIROGON_API void UnLock();

private:
	volatile LONG destination;
};

class FastLockGuard
{
public:
	FastLockGuard(FastSpinLock& spinLock);
	~FastLockGuard();

private:
	FastSpinLock& mSpinLock;
};
