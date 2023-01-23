#pragma once

class WinSocket
{
public:
	WinSocket(SOCKET socket, EProtocolType protocolType, ESocketType socketType);
	~WinSocket();

public:
	SOCKET_API bool				GetConnectionState();
	SOCKET_API bool				GetDescription();
	SOCKET_API IPAddressPtr		GetPeerAddress(const WinSocketPtr& winSocket);
	SOCKET_API uint16			GetPortNo();
	SOCKET_API EProtocolType	GetProtocolType() const;
	SOCKET_API SOCKET			GetSocket() const;
	SOCKET_API ESocketType		GetSocketType() const;

public:
	//SOCKET_API SOCKET
	SOCKET_API WinSocket*		Accept();
	SOCKET_API bool				AcceptEx(const SessionPtr& session, AcceptEvent* acceptEvent);
	SOCKET_API bool				Bind(IPAddressPtr& ipAddr);
	SOCKET_API bool				Close();
	SOCKET_API bool				Connect(IPAddressPtr& ipAddr);
	SOCKET_API bool				ConnectEx(const IPAddressPtr& ipAddr, ConnectEvent& connectEvent);
	SOCKET_API bool				DisConnect();
	SOCKET_API bool				DisConnectEx(DisconnectEvent& disconnectEvent);
	SOCKET_API bool				HasPendingConnection();
	SOCKET_API bool				HasPendingData();
	SOCKET_API bool				JoinMulticastGroup();
	SOCKET_API bool				LeaveMulticastGroup();
	SOCKET_API bool				Listen(int32 maxBacklog = SOMAXCONN);
	SOCKET_API bool				Shutdown();
	SOCKET_API bool				Wait();
	SOCKET_API bool				WaitForPendingConnection();

public:
	//RECV
	SOCKET_API bool				Recv(int8* Data, int32 BufferSize, int32& BytesRead);
	SOCKET_API bool				RecvFrom(int8* Data, int32 BufferSize, int32& BytesRead, const IPAddressPtr& ipAddr);
	SOCKET_API bool				RecvFromWithPktInfo();
	SOCKET_API bool				RecvMulti();
	SOCKET_API bool				RecvEx(RecvEvent& recvEvnet);

public:
	//SEND
	SOCKET_API bool				Send(const int8* data, int32 dataLen, int32& BytesSent);
	SOCKET_API bool				SendTo(const int8* data, int32 dataLen, int32& BytesSent, const IPAddressPtr& ipAddr);
	SOCKET_API bool				SendEx(SendEvent& sendEvent);

public:
	//SOCKET SOCKET_API OPTIONS
	SOCKET_API bool				SetBoradcast(bool bAllowBroadcast);
	SOCKET_API bool				SetIpPktInfo();
	SOCKET_API bool				SetLinger(bool bShouldLinger, int32 Timeout);
	SOCKET_API bool				SetMulticatInterface();
	SOCKET_API bool				SetMulticastLoopBack();
	SOCKET_API bool				SetMulticatTtl();
	SOCKET_API bool				SetNoDelay(bool bIsNoDelay);
	SOCKET_API bool				SetNonBlocking(bool bIsNonBlocking);
	SOCKET_API bool				SetReceiveBufferSize(int32 size);
	SOCKET_API bool				SetRecvErr();
	SOCKET_API bool				SetRetrieveTimestamp();
	SOCKET_API bool				SetReuseAddr(bool bAllowReuse);
	SOCKET_API bool				SetSendBufferSize(int32 size);
	SOCKET_API bool				UpdateAcceptSocket(SOCKET listenSocket);

	template<typename T>
	SOCKET_API bool				SetSocketOption(int32 level, int32 optName, T optVal);
protected:


protected:
	SOCKET						mSocket;
	ESocketType					mSocketType;
	EProtocolType				mProtocolType;
	uint16						mPortNo;
};