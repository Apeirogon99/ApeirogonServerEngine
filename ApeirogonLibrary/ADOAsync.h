#pragma once

using ADOCallBack = std::function<void(PacketSessionPtr&, ADOCommand&, ADORecordset&)>;

class ADOWork
{
public:
	ADOWork() {}
	ADOWork(PacketSessionPtr& inSession, ADOCommand& inADOCommand, ADORecordset& inADORecordset, ADOCallBack& inADOCallBack) : mSession(inSession), mADOCommand(inADOCommand), mADORecordset(inADORecordset), mADOCallBack(inADOCallBack) {}
	~ADOWork() {}

	ADOWork(const ADOWork& inWork) : mSession(inWork.mSession), mADOCommand(inWork.mADOCommand), mADORecordset(inWork.mADORecordset), mADOCallBack(inWork.mADOCallBack) {}
	ADOWork& operator=(const ADOWork& inWork)
	{
		mSession		= inWork.mSession;
		mADOCommand		= inWork.mADOCommand;
		mADORecordset	= inWork.mADORecordset;
		mADOCallBack	= inWork.mADOCallBack;

		return *this;
	}

protected:
	ADOWork(ADOWork&&) = delete;
	ADOWork& operator=(ADOWork&&) = delete;

public:
	PacketSessionPtr	mSession;
	ADOCommand			mADOCommand;
	ADORecordset		mADORecordset;
	ADOCallBack			mADOCallBack;
};

class ADOAsync
{
public:
	APEIROGON_API ADOAsync();
	APEIROGON_API ~ADOAsync();

private:
	ADOAsync(ADOAsync&&) = delete;
	ADOAsync(const ADOAsync&) = delete;

	ADOAsync& operator=(ADOAsync&&) = delete;
	ADOAsync& operator=(const ADOAsync&) = delete;
	
public:
	void ProcessAsync();	// 큐에 들어있는 IO들이 종료되었는지 확인

public:
	bool IsCompletionWork();
	bool GetCompeltionWork(ADOWork& outWork);

public:
	APEIROGON_API void AddWork(PacketSessionPtr& inSession, ADOCommand& inADOCommand, ADORecordset& inADORecordset, ADOCallBack& inADOCallBack);
	

private:
	CircularQueue<ADOWork>	mADOWorkQueue;
	CircularQueue<ADOWork>	mADOCompletionWorkQueue;
};