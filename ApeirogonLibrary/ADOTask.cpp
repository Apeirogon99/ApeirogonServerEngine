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
	//모든 큐에 있는 비동기 IO작업을 돌면서 끝나있다면 콜백으로 보내준다
	uint32 workCount = mADOTaskQueue.GetCount();
	for (uint32 peek = 0; peek < workCount; ++peek)
	{
		ADOItem work;
		mADOTaskQueue.Peek(work);

		if (work.mADORecordset.IsComplete())
		{
			mADOTaskQueue.Dequeue();

			//PacketSessionPtr&	session = work.mSession;
			//ADOCommand&		command = work.mADOCommand;
			//ADORecordset&		recordset = work.mADORecordset;
			//ADOCallBack&		callback = work.mADOCallBack;
			//callback(session, command, recordset);
			mADOCompletionWorkQueue.Enqueue(work);
		}
	}
}

bool ADOTask::IsCompletionWork()
{
	return mADOCompletionWorkQueue.IsEmpty() == true ? false : true;
}

bool ADOTask::GetCompeltionWork(ADOItem& outWork)
{
	ADOItem tempWork;
	if (true == mADOCompletionWorkQueue.Dequeue(tempWork))
	{
		outWork = tempWork;
		return true;
	}
	
	return false;
}

void ADOTask::AddWork(PacketSessionPtr& inSession, ADOCommand& inADOCommand, ADORecordset& inADORecordset, ADOCallBack& inADOCallBack)
{
	ADOItem newWork(inSession, inADOCommand, inADORecordset, inADOCallBack);
	mADOTaskQueue.Enqueue(newWork);
}
