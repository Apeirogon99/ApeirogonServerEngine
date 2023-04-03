#include "pch.h"
#include "ADOAsync.h"

ADOAsync::ADOAsync() : mADOWorkQueue(0xfff), mADOCompletionWorkQueue(0xfff)
{

}

ADOAsync::~ADOAsync()
{
}

void ADOAsync::ProcessAsync()
{
	//��� ť�� �ִ� �񵿱� IO�۾��� ���鼭 �����ִٸ� �ݹ����� �����ش�
	uint32 workCount = mADOWorkQueue.GetCount();
	for (uint32 peek = 0; peek < workCount; ++peek)
	{
		ADOWork work;
		mADOWorkQueue.Peek(work);

		if (work.mADORecordset.IsComplete())
		{
			mADOWorkQueue.Dequeue();

			//PacketSessionPtr&	session = work.mSession;
			//ADOCommand&		command = work.mADOCommand;
			//ADORecordset&		recordset = work.mADORecordset;
			//ADOCallBack&		callback = work.mADOCallBack;
			//callback(session, command, recordset);
			mADOCompletionWorkQueue.Enqueue(work);
		}
	}
}

bool ADOAsync::IsCompletionWork()
{
	return mADOCompletionWorkQueue.IsEmpty() == true ? false : true;
}

bool ADOAsync::GetCompeltionWork(ADOWork& outWork)
{
	ADOWork tempWork;
	if (true == mADOCompletionWorkQueue.Dequeue(tempWork))
	{
		outWork = tempWork;
		return true;
	}
	
	return false;
}

void ADOAsync::AddWork(PacketSessionPtr& inSession, ADOCommand& inADOCommand, ADORecordset& inADORecordset, ADOCallBack& inADOCallBack)
{
	ADOWork newWork(inSession, inADOCommand, inADORecordset, inADOCallBack);
	mADOWorkQueue.Enqueue(newWork);
}
