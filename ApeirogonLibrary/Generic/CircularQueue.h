#pragma once
#include <iostream>
#include <vector>
#include <type_traits>

template<typename VariableType>
class CircularQueue
{
public:
	CircularQueue(const uint16& inCapacity) : mHead(0), mTail(0)
	{
		if (inCapacity > UINT16_MAX)
		{
			throw std::invalid_argument("Capacity is too large");
		}

		//메모리 및 비트연산을 위해 2의 제곱으로 capacity설정
		uint16 newCapacity = inCapacity;
		if (!((inCapacity & (inCapacity - 1)) == 0))
		{
			newCapacity = static_cast<uint16>(pow(2, floor(log2(inCapacity)) + 1));
		}

		mCapacity = newCapacity;
		mIndexMask = newCapacity - 1;
		mQueue = new VariableType[newCapacity]();
	};

	~CircularQueue()
	{
		Clear();

		if (mQueue)
		{
			delete[] mQueue;
		}
		mQueue = nullptr;
	}

private:
	CircularQueue(CircularQueue&&) = delete;
	CircularQueue(const CircularQueue&) = delete;

	CircularQueue& operator=(CircularQueue&&) = delete;
	CircularQueue& operator=(const CircularQueue&) = delete;

public:
	void Clear()
	{
		//if (std::is_pointer_v<VariableType>) 
		//{
		//	for (uint16 index = mHead; index != mTail; index = NextIndex(index)) 
		//	{
		//		delete mQueue[index];
		//		mQueue[index] = nullptr;
		//	}
		//}
		Clear<VariableType>();
		mHead = mTail;
	}

	bool Enqueue(const VariableType& inVariable)
	{
		const uint16 currentIndex = mTail;
		const uint16 nextIndex = NextIndex(currentIndex);

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
		const uint16 currentIndex = mTail;
		const uint16 nextIndex = NextIndex(currentIndex);

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
		const uint16 currentIndex = mHead;

		if (currentIndex != mTail)
		{
			mHead = NextIndex(currentIndex);
			return true;
		}

		return false;
	}

	bool Dequeue(VariableType& outVariable)
	{
		const uint16 currentIndex = mHead;

		if (currentIndex != mTail)
		{
			outVariable = std::move(mQueue[currentIndex]);
			mHead = NextIndex(currentIndex);
			return true;
		}

		return false;
	}

	bool Dequeue(std::vector<VariableType>& outVariables)
	{
		const uint16 curUsed = GetUsedSize();
		uint16 currentIndex = mHead;

		outVariables.resize(curUsed);
		for (uint16 index = 0; index < curUsed; ++index)
		{
			outVariables[index] = std::move(mQueue[currentIndex]);
			mHead = NextIndex(currentIndex);
			currentIndex = mHead;
		}

		return true;
	}

	const VariableType* Peek() const
	{
		const uint16 currentIndex = mHead;

		if (currentIndex != mTail)
		{
			return &mQueue[currentIndex];
		}

		return nullptr;
	}

public:
	bool IsEmpty() const
	{
		return (mHead == mTail);
	}

	bool IsFull() const
	{
		return NextIndex(mTail) == mHead;
	}

	uint16 GetTotalSize() const
	{
		return mCapacity;
	}

	uint16 GetUsedSize() const
	{
		if (mTail >= mHead)
		{
			return mTail - mHead;
		}
		else
		{
			return (mCapacity - mHead) + mTail;
		}
	}

	uint16 GetFreeSize() const
	{
		return GetTotalSize() - GetUsedSize();
	}

private:
	uint16 NextIndex(uint16 inCurrent) const
	{
		return (inCurrent + 1) & mIndexMask;
	}

	template <typename U>
	void Clear(typename std::enable_if<std::is_pointer<U>::value>::type* = nullptr)
	{
		for (uint16 index = mHead; index != mTail; index = NextIndex(index))
		{
			delete mQueue[index];
			mQueue[index] = nullptr;
		}
	}

	template <typename U>
	void Clear(typename std::enable_if<!std::is_pointer<U>::value>::type* = nullptr)
	{
	}

private:
	VariableType* mQueue;
	uint16	mIndexMask;
	uint16	mCapacity;
	uint16	mHead;
	uint16	mTail;
};
