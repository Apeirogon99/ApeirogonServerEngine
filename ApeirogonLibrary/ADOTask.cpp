#include "pch.h"
#include "ADOTask.h"

DatabaseTaskQueue::DatabaseTaskQueue() : mAsyncTaskQueue(0x1000), mDatabaseTaskQueue(0x1000)
{

}

DatabaseTaskQueue::~DatabaseTaskQueue()
{
}

void DatabaseTaskQueue::ProcessAsync()
{
	FastLockGuard lockGuard(mLock);
	const ADOAsyncTaskPtr* PeekItem = mAsyncTaskQueue.Peek();
	if (nullptr == PeekItem)
	{
		return;
	}

	if (PeekItem->get()->mADOCommand.IsExecuteComplete())
	{
		ADOAsyncTaskPtr dequeueItem;
		mAsyncTaskQueue.Dequeue(dequeueItem);
		mDatabaseTaskQueue.Enqueue(std::move(dequeueItem));
	}
}

bool DatabaseTaskQueue::GetDatabaseTasks(std::vector<ADOAsyncTaskPtr>& inDatabaseTasks)
{
	FastLockGuard lockGuard(mLock);
	if (mDatabaseTaskQueue.IsEmpty())
	{
		return false;
	}

	mDatabaseTaskQueue.Dequeue(inDatabaseTasks);
	return true;
}

bool DatabaseTaskQueue::PushAsyncTaskQueue(PacketSessionPtr& inSession, ADOConnection& inADOConnection, ADOCommand& inADOCommand, ADORecordset& inADORecordset, ADOCallBack& inADOCallBack)
{
	FastLockGuard lockGuard(mLock);
	ADOAsyncTaskPtr newItem = std::make_shared<ADOAsyncTask>(inSession, inADOConnection, inADOCommand, inADORecordset, inADOCallBack);
	const bool result = mAsyncTaskQueue.Enqueue(std::move(newItem));

	return result;
}
