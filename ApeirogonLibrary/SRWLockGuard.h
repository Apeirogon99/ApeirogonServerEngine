#pragma once

enum class ESRWLockMode
{
	Read,
	Write
};

class SimpleSRWLock
{
public:
	SimpleSRWLock() { InitializeSRWLock(&mSRWLock); }
	~SimpleSRWLock() {}

protected:
	SimpleSRWLock(const SimpleSRWLock&) = delete;
	SimpleSRWLock(SimpleSRWLock&&) = delete;

	SimpleSRWLock operator=(const SimpleSRWLock&) = delete;
	SimpleSRWLock operator=(SimpleSRWLock&&) = delete;

public:
	void Lock(const ESRWLockMode inMode)
	{
		switch (inMode)
		{
		case ESRWLockMode::Read:
			AcquireSRWLockShared(&this->mSRWLock);
			break;
		case ESRWLockMode::Write:
			AcquireSRWLockExclusive(&this->mSRWLock);
			break;
		default:
			break;
		}
	}

	void UnLock(const ESRWLockMode inMode)
	{
		switch (inMode)
		{
		case ESRWLockMode::Read:
			ReleaseSRWLockShared(&this->mSRWLock);
			break;
		case ESRWLockMode::Write:
			ReleaseSRWLockExclusive(&this->mSRWLock);
			break;
		default:
			break;
		}
	}

	bool TryLock(const ESRWLockMode inMode)
	{
		switch (inMode)
		{
		case ESRWLockMode::Read:
			return TryAcquireSRWLockShared(&this->mSRWLock);
			break;
		case ESRWLockMode::Write:
			return TryAcquireSRWLockExclusive(&this->mSRWLock);
			break;
		default:
			break;
		}

		return false;
	}

private:
	SRWLOCK mSRWLock;
};

class SRWLockGuard
{
public:
	SRWLockGuard(SimpleSRWLock& inSimpleSRWLock, const ESRWLockMode inMode) : mSimpleSRWLock(inSimpleSRWLock), mSRWLockMode(inMode)
	{
		mSimpleSRWLock.Lock(mSRWLockMode);
	}

	~SRWLockGuard()
	{
		mSimpleSRWLock.UnLock(mSRWLockMode);
	}

protected:
	SRWLockGuard(const SRWLockGuard&) = delete;
	SRWLockGuard(SRWLockGuard&&) = delete;

	SRWLockGuard operator=(const SRWLockGuard&) = delete;
	SRWLockGuard operator=(SRWLockGuard&&) = delete;


private:
	ESRWLockMode	mSRWLockMode;
	SimpleSRWLock&	mSimpleSRWLock;
};