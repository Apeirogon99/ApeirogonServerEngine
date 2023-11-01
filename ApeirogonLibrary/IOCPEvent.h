#pragma once

enum class EventType : uint8
{
	Connect,
	Disconnect,
	Accept,
	//PreRecv,
	Recv,
	Send,
	RegisterSend,
	Icmp,
	Exit,
};

/*--------------
	IocpEvent
---------------*/

class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);

	void			Init();
	void			SetOwner(IOCPObjectPtr inOwner);

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
	void SetSession(SessionPtr inSession);

public:
	SessionPtr	session = nullptr;
};

/*----------------
	RecvEvent
-----------------*/

class RecvEvent : public IocpEvent
{
	enum
	{
		RECV_MAX_BUFFER = 0xffff,
	};

public:
	RecvEvent() : IocpEvent(EventType::Recv)
	{
		Clean(); 
		mRecvBuffer.InitBuffer(RECV_MAX_BUFFER);
	}
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

class RegisterSendEvent : public IocpEvent
{
public:
	RegisterSendEvent() : IocpEvent(EventType::RegisterSend) { }
};

class IcmpEvent : public IocpEvent
{
	enum
	{
		ICMP_MAX_BUFFER		= 0xff,
		ICMP_MAX_TIMEOUT	= 0x1F4,
	};

public:
	IcmpEvent() : IocpEvent(EventType::Icmp), mTimeout(ICMP_MAX_TIMEOUT)
	{ 
		mReplyBuffer.InitBuffer(ICMP_MAX_BUFFER);
	}
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

class ExitEvent : public IocpEvent
{
public:
	ExitEvent() : IocpEvent(EventType::Exit) { }
};