#pragma once

enum class EventType : uint8
{
	Connect,
	Disconnect,
	Accept,
	//PreRecv,
	Recv,
	Send,
	Icmp,
};

/*--------------
	IocpEvent
---------------*/

class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);

	void			Init();

public:
	EventType		eventType;
	IOCPObjectPtr	owner;
};

/*----------------
	ConnectEvent
-----------------*/

class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect) { }
};

/*--------------------
	DisconnectEvent
----------------------*/

class DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent() : IocpEvent(EventType::Disconnect) { }
};

/*----------------
	AcceptEvent
-----------------*/

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) { }

public:
	SessionPtr	session = nullptr;
};

/*----------------
	RecvEvent
-----------------*/

class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv), mRecvBuffer(0xffff) { Clean(); }
	void Clean() { mRecvBuffer.Clean(); }
	
public:
	RingBuffer mRecvBuffer;
};

/*----------------
	SendEvent
-----------------*/

class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) { }
	void Clean() { sendBuffers.clear(); }

	//CircularQueue<SendBufferPtr> mSendQeue;
	std::vector<SendBufferPtr> sendBuffers;
};

class IcmpEvent : public IocpEvent
{
	enum
	{
		ICMP_MAX_BUFFER		= 0xff,
		ICMP_MAX_TIMEOUT	= 0x1F4,
	};

public:
	IcmpEvent() : IocpEvent(EventType::Icmp), mReplyBuffer(ICMP_MAX_BUFFER), mTimeout(ICMP_MAX_TIMEOUT){ }
	void Clean() 
	{ 
		mReplyBuffer.Clean();
		mIpAddr.reset();
	}

public:
	IPAddressPtr	mIpAddr;
	RingBuffer		mReplyBuffer;
	uint32			mTimeout;
};