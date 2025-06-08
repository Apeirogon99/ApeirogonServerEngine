#pragma once

using ADOCallBack = std::function<bool(PacketSessionPtr&, ADOConnection&, ADOCommand&, ADORecordset&)>;

class ADOAsyncTask
{
public:
	ADOAsyncTask()
	{

	}

	ADOAsyncTask(PacketSessionPtr& inSession, ADOConnection& inADOConnection, ADOCommand& inADOCommand, ADORecordset& inADORecordset, ADOCallBack& inADOCallBack)
	{
		mSession		= inSession;
		mADOConnection	= inADOConnection;
		mADOCommand		= inADOCommand;
		mADORecordset	= inADORecordset;
		mADOCallBack	= inADOCallBack;
	}
	~ADOAsyncTask()
	{
		Release();
	}

	ADOAsyncTask(const ADOAsyncTask& inWork)
	{
		mSession		= inWork.mSession;
		mADOConnection	= inWork.mADOConnection;
		mADOCommand		= inWork.mADOCommand;
		mADORecordset	= inWork.mADORecordset;
		mADOCallBack	= inWork.mADOCallBack;
	}
	ADOAsyncTask& operator=(const ADOAsyncTask& inWork)
	{
		mSession		= inWork.mSession;
		mADOConnection	= inWork.mADOConnection;
		mADOCommand		= inWork.mADOCommand;
		mADORecordset	= inWork.mADORecordset;
		mADOCallBack	= inWork.mADOCallBack;
		return *this;
	}

protected:
	ADOAsyncTask(ADOAsyncTask&&) = delete;
	ADOAsyncTask& operator=(ADOAsyncTask&&) = delete;

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