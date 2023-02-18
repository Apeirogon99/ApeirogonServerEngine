#include "pch.h"
#include "SendQueue.h"

SendQueue::SendQueue() : mQueueSize(static_cast<uint32>(Default::MAX_QUEUE_SIZE)), mFront(0), mRear(0)
{
}

SendQueue::~SendQueue()
{
}

void SendQueue::Push(SendBufferPtr sendBuffer)
{
	FastLockGuard lockGuard(mFastSpinLock);

	if (true == IsFull())
	{
		wprintf(L"SendQueue::Push() : send queue is full\n");
		return;
	}

	mBuffer[mFront] = sendBuffer;
	mFront = (mFront + 1) % mQueueSize;
}

void SendQueue::PopAll(std::vector<SendBufferPtr>& sendBuffers)
{
	FastLockGuard lockGuard(mFastSpinLock);
	while (mRear != mFront)
	{
		SendBufferPtr popBuffer = mBuffer[mRear];
		sendBuffers.push_back(popBuffer);
		mBuffer[mRear].reset();
		mRear = (mRear + 1) % mQueueSize;
	}
}

bool SendQueue::IsFull()
{
	const uint32 isFull = (mFront + 1) % mQueueSize;
	return isFull == mRear ? true : false;
}

bool SendQueue::IsEmpty()
{
	FastLockGuard lockGuard(mFastSpinLock);
	return mRear == mFront ? true : false;
}
