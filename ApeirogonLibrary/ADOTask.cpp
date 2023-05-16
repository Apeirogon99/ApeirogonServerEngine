#include "pch.h"
#include "ADOTask.h"

ADOTask::ADOTask() : mADOTaskQueue(0xfff), mADOCompletionWorkQueue(0xfff)
{

}

ADOTask::~ADOTask()
{
}

void ADOTask::ProcessAsync()
{

	FastLockGuard lockGuard(mLock);
	const ADOItemPtr* PeekItem = mADOTaskQueue.Peek();
	if (nullptr == PeekItem)
	{
		return;
	}

	if (PeekItem->get()->mADOCommand.IsExecuteComplete())
	{
		ADOItemPtr dequeueItem;
		mADOTaskQueue.Dequeue(dequeueItem);
		mADOCompletionWorkQueue.Enqueue(std::move(dequeueItem));
	}
}

bool ADOTask::IsCompletionWork()
{
	return mADOCompletionWorkQueue.IsEmpty() == true ? false : true;
}

bool ADOTask::GetCompeltionWork(ADOItemPtr& outWork)
{
	ADOItemPtr tempWork;
	FastLockGuard lockGuard(mLock);
	if (true == mADOCompletionWorkQueue.Dequeue(tempWork))
	{
		outWork = std::move(tempWork);
		return true;
	}
	
	return false;
}

void ADOTask::AddWork(PacketSessionPtr& inSession, ADOConnection& inADOConnection, ADOCommand& inADOCommand, ADORecordset& inADORecordset, ADOCallBack& inADOCallBack)
{
	FastLockGuard lockGuard(mLock);
	
	ADOItemPtr newWork = std::make_shared<ADOItem>(inSession, inADOConnection, inADOCommand, inADORecordset, inADOCallBack);
	mADOTaskQueue.Enqueue(std::move(newWork));
}
