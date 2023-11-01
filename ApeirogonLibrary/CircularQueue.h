#pragma once

template<typename VariableType>
class CircularQueue
{
public:
	CircularQueue(const uint32 inCapcity) : mHead(0), mTail(0)
	{
		//작업의 효율성을 위해 2의 제곱의 크기로 Capcity의 크기를 증가시켜야함
		if (inCapcity > UINT16_MAX)
		{
			wprintf(L"[CircularQueue] It's too big to make");
		}

		int32 newCapcity = 0;
		if (!((inCapcity & (inCapcity - 1)) == 0))
		{
			newCapcity = static_cast<int32>(pow(2, floor(log2(inCapcity)) + 1));
		}
		else
		{
			newCapcity = static_cast<int32>(inCapcity);
		}

		mCapcity = newCapcity;
		mIndexMask = newCapcity - 1;
		mQueue = new VariableType[newCapcity]();
	};

	~CircularQueue()
	{
		if (mQueue)
		{
			delete[] mQueue;
		}
		mQueue = nullptr;
	}

	CircularQueue(CircularQueue&&) = delete;
	CircularQueue(const CircularQueue&) = delete;

	CircularQueue& operator=(CircularQueue&&) = delete;
	CircularQueue& operator=(const CircularQueue&) = delete;

public:
	void Clear()
	{
		mHead = mTail;
		memset(mQueue, NULL, mCapcity);
		//mHead.store(mTail.load());
	}

	bool Enqueue(const VariableType& inVariable)
	{

		//SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Write);

		const int32 currentIndex = mTail;
		const int32 nextIndex = ((currentIndex + 1) & mIndexMask);

		if (nextIndex != mHead)
		{
			mQueue[currentIndex] = inVariable;
			mTail = nextIndex;

			return true;
		}

		return false;
	}

	bool Enqueue(VariableType&& inVariable)
	{
		//SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Write);

		const int32 currentIndex = mTail;
		const int32 nextIndex = ((currentIndex + 1) & mIndexMask);

		if (nextIndex != mHead)
		{
			mQueue[currentIndex] = std::move(inVariable);
			mTail = nextIndex;
			return true;
		}

		return false;
	}

	bool Dequeue()
	{
		//SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Write);

		const int32 currentIndex = mHead;

		if (currentIndex != mTail)
		{
			mHead = ((currentIndex + 0b1) & mIndexMask);
			return true;
		}

		return false;
	}

	bool Dequeue(VariableType& outVariable)
	{
		//SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Write);

		const int32 currentIndex = mHead;

		if (currentIndex != mTail)
		{
			outVariable = std::move(mQueue[currentIndex]);
			mHead = ((currentIndex + 0b1) & mIndexMask);
			return true;
		}

		return false;
	}

	bool Dequeue(VariableType outVariables[], const uint32 inNumber)
	{
		//SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Write);
		if (inNumber > GetUsedSize())
		{
			return false;
		}

		if (inNumber != (_msize(outVariables) / sizeof(VariableType)))
		{
			return false;
		}

		int32 currentIndex = mHead;

		for (int32 index = 0; index < inNumber; ++index)
		{
			outVariables[index] = std::move(mQueue[currentIndex]);
			mHead = ((currentIndex + 0b1) & mIndexMask);
			currentIndex = mHead;
		}

		return true;
	}

	bool Dequeue(std::vector<VariableType>& outVariables)
	{
		//SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Write);

		const int32 curUsed = GetUsedSize();
		int32 currentIndex = mHead;

		outVariables.resize(curUsed);
		for (int32 index = 0; index < curUsed; ++index)
		{
			outVariables[index] = std::move(mQueue[currentIndex]);
			mHead = ((currentIndex + 0b1) & mIndexMask);
			currentIndex = mHead;
		}

		return true;
	}

	bool Peek(VariableType& outVariable)
	{
		//SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Read);

		const int32 currentIndex = mHead;

		if (currentIndex != mTail)
		{
			outVariable = mQueue[currentIndex];
			return true;
		}

		return false;
	}

	bool Peek(std::vector<VariableType>& outVariables)
	{
		//SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Write);

		const int32 curUsed = GetUsedSize();
		int32 currentIndex = mHead;

		outVariables.resize(curUsed);
		for (int32 index = 0; index < curUsed; ++index)
		{
			outVariables[index] = mQueue[currentIndex];
			currentIndex = ((currentIndex + 0b1) & mIndexMask);
		}

		return true;
	}



	const VariableType* Peek()
	{
		//SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Read);

		const int32 currentIndex = mHead;

		if (currentIndex != mTail)
		{
			return &mQueue[currentIndex];
		}

		return nullptr;
	}

public:
	bool IsEmpty()
	{
		//SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Read);
		return (mHead == mTail);
	}

	bool IsFull()
	{
		//SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Read);
		return (((mTail + 0b1) & mIndexMask) == mHead);
	}

	int32 GetCount()
	{
		//SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Read);
		int32 Count = mTail - mHead;

		if (Count < 0)
		{
			Count += mCapcity;
		}

		return Count;
	}

	int32 GetCapcity() const
	{
		return mCapcity;
	}

public:
	int32 GetUsedSize()
	{
		int32 Count = 0;
		Count = mTail - mHead;
		if (Count < 0)
		{
			Count += mCapcity;
		}

		return Count;
	}

	int32 GetTotalSize()
	{
		return mCapcity;
	}

	int32 GetFreeSize()
	{
		return GetTotalSize() - GetUsedSize();
	}

	int32 GetDoOnceSize()
	{
		if (mHead > mTail)
		{
			return GetFreeSize();
		}

		return mCapcity - mTail;
	}

private:
	VariableType* mQueue;
	//SimpleSRWLock	mSRWLock;
	int32	mIndexMask;
	int32	mCapcity;
	int32	mHead;
	int32	mTail;
};

