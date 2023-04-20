#pragma once

using ADOCallBack = std::function<void(PacketSessionPtr&, ADOCommand&, ADORecordset&)>;

class ADOItem
{
public:
	ADOItem() {}
	ADOItem(PacketSessionPtr& inSession, ADOCommand& inADOCommand, ADORecordset& inADORecordset, ADOCallBack& inADOCallBack) : mSession(inSession), mADOCommand(inADOCommand), mADORecordset(inADORecordset), mADOCallBack(inADOCallBack) {}
	~ADOItem() {}

	ADOItem(const ADOItem& inWork) : mSession(inWork.mSession), mADOCommand(inWork.mADOCommand), mADORecordset(inWork.mADORecordset), mADOCallBack(inWork.mADOCallBack) {}
	ADOItem& operator=(const ADOItem& inWork)
	{
		mSession		= inWork.mSession;
		mADOCommand		= inWork.mADOCommand;
		mADORecordset	= inWork.mADORecordset;
		mADOCallBack	= inWork.mADOCallBack;

		return *this;
	}

protected:
	ADOItem(ADOItem&&) = delete;
	ADOItem& operator=(ADOItem&&) = delete;

public:
	PacketSessionPtr	mSession;
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
	bool GetCompeltionWork(ADOItem& outWork);

public:
	APEIROGON_API void AddWork(PacketSessionPtr& inSession, ADOCommand& inADOCommand, ADORecordset& inADORecordset, ADOCallBack& inADOCallBack);
	

private:
	CircularQueue<ADOItem>	mADOTaskQueue;
	CircularQueue<ADOItem>	mADOCompletionWorkQueue;
};