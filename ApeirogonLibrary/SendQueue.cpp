#include "pch.h"
#include "SendQueue.h"
#include "CircularQueue.h"

SendQueue::SendQueue() : mSendQueue(static_cast<uint32>(Default::MAX_QUEUE_SIZE))
{
	
}

SendQueue::~SendQueue()
{
}

void SendQueue::Clear()
{
	FastLockGuard lockGuard(mLock);
	const uint32 count = mSendQueue.GetCount();
	for (uint32 index = 0; index < count; ++index)
	{
		SendBufferPtr SendBuffer;
		mSendQueue.Dequeue(SendBuffer);
		SendBuffer.reset();
	}
}

void SendQueue::Push(SendBufferPtr& inSendBuffer)
{
	FastLockGuard lockGuard(mLock);
	if (false == mSendQueue.Enqueue(std::move(inSendBuffer)))
	{
		wprintf(L"[SendQueue::Push] Push Error");
	}
}

void SendQueue::Pop(SendBufferPtr& OutSendBuffer)
{
	FastLockGuard lockGuard(mLock);
	if (false == mSendQueue.Dequeue(OutSendBuffer))
	{
		wprintf(L"[SendQueue::Pop] Pop Error");
		return;
	}
}

void SendQueue::Pop(std::vector<SendBufferPtr>& OutSendBuffers)
{
	FastLockGuard lockGuard(mLock);
	if (false == mSendQueue.Dequeue(OutSendBuffers))
	{
		wprintf(L"[SendQueue::Pop] PopAll Error");
		return;
	}
}

bool SendQueue::IsEmpty()
{
	FastLockGuard lockGuard(mLock);
	return mSendQueue.IsEmpty();
}

bool SendQueue::IsFull()
{
	FastLockGuard lockGuard(mLock);
	return mSendQueue.IsFull();
}