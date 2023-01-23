#include "pch.h"
#include "FastSpinLock.h"

FastSpinLock::FastSpinLock() : destination(static_cast<LONG>(Default::LOCK_IS_FREE))
{
}

FastSpinLock::~FastSpinLock()
{
}

void FastSpinLock::Lock()
{
	LONG oldCount = static_cast<LONG>(Default::LOCK_IS_TAKEN);

	while (true)
	{
		oldCount = _InterlockedCompareExchange(&destination, static_cast<LONG>(Default::LOCK_IS_TAKEN), static_cast<LONG>(Default::LOCK_IS_FREE));
		if (oldCount)
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
	destination = static_cast<LONG>(Default::LOCK_IS_FREE);
}

FastLockGuard::FastLockGuard(FastSpinLock& spinLock) : mSpinLock(spinLock)
{
	mSpinLock.Lock();
}

FastLockGuard::~FastLockGuard()
{
	mSpinLock.UnLock();
}
