#pragma once

using ADOCallBack = std::function<void(PacketSessionPtr&, ADOConnection&, ADOCommand&, ADORecordset&)>;

class ADOItem
{
public:
	ADOItem()
	{

	}

	ADOItem(PacketSessionPtr& inSession, ADOConnection& inADOConnection, ADOCommand& inADOCommand, ADORecordset& inADORecordset, ADOCallBack& inADOCallBack)
	{
		mSession		= inSession;
		mADOConnection	= inADOConnection;
		mADOCommand		= inADOCommand;
		mADORecordset	= inADORecordset;
		mADOCallBack	= inADOCallBack;
	}
	~ADOItem() 
	{
		Release();
	}

	ADOItem(const ADOItem& inWork)
	{
		mSession		= inWork.mSession;
		mADOConnection	= inWork.mADOConnection;
		mADOCommand		= inWork.mADOCommand;
		mADORecordset	= inWork.mADORecordset;
		mADOCallBack	= inWork.mADOCallBack;
	}
	ADOItem& operator=(const ADOItem& inWork)
	{
		mSession		= inWork.mSession;
		mADOConnection	= inWork.mADOConnection;
		mADOCommand		= inWork.mADOCommand;
		mADORecordset	= inWork.mADORecordset;
		mADOCallBack	= inWork.mADOCallBack;
		return *this;
	}

protected:
	ADOItem(ADOItem&&) = delete;
	ADOItem& operator=(ADOItem&&) = delete;

public:
	void Execute()
	{
		mADOCallBack(mSession, mADOConnection, mADOCommand, mADORecordset);
	}

	void Release()
	{
		mSession.reset();

		if (mADORecordset.IsOpen())
		{
			mADORecordset.Close();
		}
		mADORecordset.Release();

		mADOCommand.Release();

		if (mADOConnection.IsOpen())
		{
			mADOConnection.Close();
		}
		mADOConnection.Release();
		mADOCallBack = NULL;
	}

public:
	PacketSessionPtr	mSession;
	ADOConnection		mADOConnection;
	ADOCommand			mADOCommand;
	ADORecordset		mADORecordset;
	ADOCallBack			mADOCallBack;
};

class ADOTask
{
public:
	APEIROGON_API ADOTask();
	APEIROGON_API ~ADOTask();

private:
	ADOTask(ADOTask&&) = delete;
	ADOTask(const ADOTask&) = delete;

	ADOTask& operator=(ADOTask&&) = delete;
	ADOTask& operator=(const ADOTask&) = delete;
	
public:
	void ProcessAsync();	// 큐에 들어있는 IO들이 종료되었는지 확인

public:
	bool IsCompletionWork();
	bool GetCompeltionWork(ADOItemPtr& outWork);

public:
	APEIROGON_API void AddWork(PacketSessionPtr& inSession, ADOConnection& inADOConnection, ADOCommand& inADOCommand, ADORecordset& inADORecordset, ADOCallBack& inADOCallBack);
	

private:
	FastSpinLock				mLock;
	CircularQueue<ADOItemPtr>	mADOTaskQueue;
	CircularQueue<ADOItemPtr>	mADOCompletionWorkQueue;
};