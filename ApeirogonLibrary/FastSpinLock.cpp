#include "pch.h"
#include "FastSpinLock.h"

FastSpinLock::FastSpinLock()
{
	_InterlockedExchange(&destination, static_cast<LONG>(Default::LOCK_IS_FREE));
}

FastSpinLock::~FastSpinLock()
{
}

void FastSpinLock::Lock()
{
	LONG lockState = static_cast<LONG>(Default::LOCK_IS_TAKEN);
	while (true)
	{
		lockState = _InterlockedCompareExchange(&destination, static_cast<LONG>(Default::LOCK_IS_TAKEN), static_cast<LONG>(Default::LOCK_IS_FREE));
		if (lockState == static_cast<LONG>(Default::LOCK_IS_TAKEN))
		{
			for (DWORD i = 0; i < static_cast<DWORD>(Default::MAX_SPIN_COUNT); ++i)
			{
				YieldProcessor();
			}
		}
		else
		{
			return;
		}
	}
}

void FastSpinLock::UnLock()
{
	_InterlockedExchange(&destination, static_cast<LONG>(Default::LOCK_IS_FREE));
}

FastLockGuard::FastLockGuard(FastSpinLock& spinLock) : mFastSpinLock(spinLock)
{
	this->mFastSpinLock.Lock();
}

FastLockGuard::~FastLockGuard()
{
	this->mFastSpinLock.UnLock();
}
