#pragma once
class LockController
{
public:
	APEIROGON_API LockController();
	APEIROGON_API ~LockController();

	LockController(const LockController&) = delete;
	LockController& operator=(const LockController&) = delete;

public:
	void Lock();
	void UnLock();

	APEIROGON_API void Wait(DWORD inTimeOut = INFINITE);
	APEIROGON_API void NotifyOne();
	APEIROGON_API void NotifyAll();

private:
	CONDITION_VARIABLE	mCondition;
	CRITICAL_SECTION	mCirticalSesction;
};

class LockGuardController
{
public:
	APEIROGON_API LockGuardController(LockController& inLockController);
	APEIROGON_API ~LockGuardController();

	LockGuardController(const LockGuardController&) = delete;
	LockGuardController(LockGuardController&&) = delete;
	LockGuardController& operator=(const LockGuardController&) = delete;
	LockGuardController& operator=(LockGuardController&&) = delete;

private:
	LockController& mLockController;
};
