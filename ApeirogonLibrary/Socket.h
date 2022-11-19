#pragma once

#ifdef DLL_DEVELOPMENT
#define SOCKET_API __declspec(dllimport)
#else
#define SOCKET_API __declspec(dllexport)
#endif // DLL_DEVELOPMENT

class SOCKET_API Socket
{
public:
	Socket(SOCKET socket, EProtocolType protocolType, ESocketType socketType);
	~Socket();

public:
	//Set
	void SetAddress(IPAddress& ipAddr);

public:
	//Get
	IPAddress		GetAddress();
	bool			GetConnectionState();
	bool			GetDescription();
	bool			GetPeerAddress();
	uint16			GetPortNo();
	EProtocolType	GetProtocolType() const;
	SOCKET			GetSocket() const;
	ESocketType		GetSocketType() const;

public:
	//SOCKET
	Socket* Accept();
	bool Bind(IPAddress& ipAddr);
	bool Close();
	bool Connect(IPAddress& ipAddr);
	bool HasPendingConnection();
	bool HasPendingData();
	bool JoinMulticastGroup();
	bool LeaveMulticastGroup();
	bool Listen(int32 maxBacklog = SOMAXCONN);
	bool Shutdown();
	bool Wait();
	bool WaitForPendingConnection();

public:
	//RECV
	bool Recv(int8* Data, int32 BufferSize, int32& BytesRead);
	bool RecvFrom();
	bool RecvFromWithPktInfo();
	bool RecvMulti();
	bool RecvEx();

public:
	//SEND
	bool Send(const int8* data, int32 dataLen, int32& BytesSent);
	bool SendTo();
	bool SendEx();

public:
	//SOCKET OPTIONS
	bool SetBoradcast(bool bAllowBroadcast);
	bool SetIpPktInfo();
	bool SetLinger(bool bShouldLinger, int32 Timeout);
	bool SetMulticatInterface();
	bool SetMulticastLoopBack();
	bool SetMulticatTtl();
	bool SetNoDelay(bool bIsNoDelay);
	bool SetNonBlocking(bool bIsNonBlocking);
	bool SetReceiveBufferSize(int32 size);
	bool SetRecvErr();
	bool SetRetrieveTimestamp();
	bool SetReuseAddr(bool bAllowReuse);
	bool SetSendBufferSize(int32 size);
	bool UpdateAcceptSocket(SOCKET listenSocket);

	template<typename T>
	bool SetSocketOption(int32 level, int32 optName, T optVal);

protected:

protected:
	SOCKET			mSocket;
	ESocketType		mSocketType;
	EProtocolType	mProtocolType;
	uint16			mPortNo;
	IPAddress		mIpAddress;
};