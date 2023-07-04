#include "pch.h"
#include "LockController.h"

LockController::LockController()
{
	InitializeConditionVariable(&mCondition);
	InitializeCriticalSection(&mCirticalSesction);
}

LockController::~LockController()
{
	
}

void LockController::Lock()
{
	EnterCriticalSection(&mCirticalSesction);
}

void LockController::UnLock()
{
	LeaveCriticalSection(&mCirticalSesction);
}

void LockController::Wait(DWORD inTimeOut)
{
	SleepConditionVariableCS(&mCondition, &mCirticalSesction, inTimeOut);
}

void LockController::NotifyOne()
{
	WakeConditionVariable(&mCondition);
}

void LockController::NotifyAll()
{
	WakeAllConditionVariable(&mCondition);
}

LockGuardController::LockGuardController(LockController& inLockController) : mLockController(inLockController)
{
	mLockController.Lock();
}

LockGuardController::~LockGuardController()
{
	mLockController.UnLock();
}
