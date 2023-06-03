#pragma once

class FastSpinLock
{
	enum class Default
	{
		MAX_SPIN_COUNT = 0x64,
		LOCK_IS_TAKEN = 1,
		LOCK_IS_FREE = 0,
	};

public:
	APEIROGON_API FastSpinLock();
	APEIROGON_API ~FastSpinLock();

	FastSpinLock(const FastSpinLock& inFastSpinLock) = delete;
	FastSpinLock& operator=(const FastSpinLock& inFastSpinLock) = delete;

public:
	APEIROGON_API void Lock();
	APEIROGON_API void UnLock();

private:
	volatile LONG destination;
};

class FastLockGuard
{
public:
	APEIROGON_API FastLockGuard(FastSpinLock& spinLock);
	APEIROGON_API ~FastLockGuard();

private:
	FastSpinLock& mFastSpinLock;
};
