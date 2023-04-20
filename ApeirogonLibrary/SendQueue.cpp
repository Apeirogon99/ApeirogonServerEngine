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
	if (false == mSendQueue.Enqueue(std::move(inSendBuffer)))
	{
		wprintf(L"[SendQueue::Push] Push Error");
	}
}

void SendQueue::Pop(SendBufferPtr& OutSendBuffer)
{
	SendBufferPtr tempSendBuffer;
	if (false == mSendQueue.Dequeue(tempSendBuffer))
	{
		wprintf(L"[SendQueue::Pop] Pop Error");
		return;
	}

	OutSendBuffer = std::move(tempSendBuffer);
}

void SendQueue::Pop(std::vector<SendBufferPtr> OutSendBuffers)
{
	if (false == mSendQueue.Dequeue(OutSendBuffers))
	{
		wprintf(L"[SendQueue::Pop] PopAll Error");
		return;
	}
}

bool SendQueue::IsEmpty()
{
	return mSendQueue.IsEmpty();
}

bool SendQueue::IsFull()
{
	return mSendQueue.IsFull();
}