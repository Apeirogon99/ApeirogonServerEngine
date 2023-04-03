#pragma once

template<typename VariableType>
class CircularQueue
{
public:
	CircularQueue(uint32 inCapcity) : mHead(0), mTail(0)
	{
		//작업의 효율성을 위해 2의 제곱의 크기로 Capcity의 크기를 증가시켜야함
		uint32 newCapcity = static_cast<uint32>(pow(2, floor(log2(inCapcity)) + 1));
		mCapcity = newCapcity;
		mIndexMask = newCapcity - 1;
		mQueue = new VariableType[newCapcity]();
	};

	~CircularQueue()
	{
		delete[] mQueue;
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

		SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Write);

		const uint32 currentIndex = mTail;
		const uint32 nextIndex = ((currentIndex + 1) & mIndexMask);

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
		SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Write);

		const uint32 currentIndex = mTail;
		const uint32 nextIndex = ((currentIndex + 1) & mIndexMask);

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
		SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Write);

		const uint32 currentIndex = mHead;

		if (currentIndex != mTail)
		{
			mHead = ((currentIndex + 0b1) & mIndexMask);
			return true;
		}

		return false;
	}

	bool Dequeue(VariableType& outVariable)
	{
		SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Write);

		const uint32 currentIndex = mHead;

		if (currentIndex != mTail)
		{
			outVariable = mQueue[currentIndex];
			mHead = ((currentIndex + 0b1) & mIndexMask);

			return true;
		}

		return false;
	}

	bool Dequeue(VariableType outVariables[], const uint32 inNumber)
	{
		SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Write);
		if (inNumber > GetUsedSize())
		{
			return false;
		}

		if (inNumber != (_msize(outVariables) / sizeof(VariableType)))
		{
			return false;
		}

		uint32 currentIndex = mHead;

		for (uint32 index = 0; index < inNumber; ++index)
		{
			outVariables[index] = std::move(mQueue[currentIndex]);
			mHead = ((currentIndex + 0b1) & mIndexMask);
			currentIndex = mHead;
		}

		return true;
	}

	//bool DequeueAll(VariableType outVariables[], uint32& outNumber)
	//{
	//	SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Write);

	//	const uint32 usedSize = GetUsedSize();
	//	outVariables = new VariableType[usedSize]();

	//	const uint32 currentIndex = mHead;
	//	uint32 index = 0;

	//	while (mHead != mTail)
	//	{
	//		const uint32 nextIndex = currentIndex + index++;
	//		//outVariables[index++] = std::move(mQueue[nextIndex]);
	//		mHead = ((nextIndex + 0b1) & mIndexMask);
	//	}

	//	outNumber = index;
	//	return true;
	//}

	bool Peek(VariableType& outVariable)
	{
		SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Read);

		const uint32 currentIndex = mHead;

		if (currentIndex != mTail)
		{
			outVariable = mQueue[currentIndex];
			return true;
		}

		return false;
	}



	const VariableType* Peek()
	{
		SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Read);

		const uint32 currentIndex = mHead;

		if (currentIndex != mTail)
		{
			return &mQueue[currentIndex];
		}

		return nullptr;
	}

public:
	bool IsEmpty()
	{
		SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Read);
		return (mHead == mTail);
	}

	bool IsFull()
	{
		SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Read);
		return (((mTail + 0b1) & mIndexMask) == mHead);
	}

	uint32 GetCount()
	{
		SRWLockGuard lockGuard(mSRWLock, ESRWLockMode::Read);
		uint32 Count = mTail - mHead;

		if (Count < 0)
		{
			Count += mCapcity;
		}

		return Count;
	}

protected:
	uint32 GetUsedSize()
	{
		uint32 Count = mTail - mHead;

		if (Count < 0)
		{
			Count += mCapcity;
		}

		return Count;
	}

	uint32 GetTotalSize()
	{
		return mCapcity;
	}

	uint32 GetFreeSize()
	{
		return GetTotalSize() - GetUsedSize();
	}

	uint32 GetDoOnceSize()
	{
		if (mHead > mTail)
		{
			return GetFreeSize();
		}

		return mCapcity - mTail;
	}

private:
	VariableType* mQueue;
	SimpleSRWLock	mSRWLock;
	uint32	mIndexMask;
	uint32	mCapcity;
	uint32	mHead;
	uint32	mTail;
};

