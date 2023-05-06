#pragma once

class WinSocket
{
public:
	WinSocket(SOCKET socket, EProtocolType protocolType, ESocketType socketType);
	~WinSocket();

public:
	APEIROGON_API bool				GetConnectionState();
	APEIROGON_API bool				GetDescription();
	APEIROGON_API IPAddressPtr		GetPeerAddress(const WinSocketPtr& winSocket);
	APEIROGON_API uint16			GetPortNo();
	APEIROGON_API EProtocolType		GetProtocolType() const;
	APEIROGON_API SOCKET			GetSocket() const;
	APEIROGON_API ESocketType		GetSocketType() const;

public:
	//APEIROGON_API SOCKET
	APEIROGON_API WinSocket*		Accept();
	APEIROGON_API bool				AcceptEx(const SessionPtr& session, AcceptEvent* acceptEvent);
	APEIROGON_API bool				Bind(IPAddressPtr& ipAddr);
	APEIROGON_API bool				Close();
	APEIROGON_API bool				Connect(IPAddressPtr& ipAddr);
	APEIROGON_API bool				ConnectEx(const IPAddressPtr& ipAddr, ConnectEvent& connectEvent);
	APEIROGON_API bool				DisConnect();
	APEIROGON_API bool				DisConnectEx(DisconnectEvent& disconnectEvent);
	APEIROGON_API bool				Listen(int32 maxBacklog = SOMAXCONN);
	APEIROGON_API bool				Shutdown();
	APEIROGON_API bool				Wait(const int32 waitTime);
	APEIROGON_API bool				WaitForPendingConnection(const int32 waitTime = 0);

public:
	//RECV
	APEIROGON_API bool				Recv(int8* Data, int32 BufferSize, int32& BytesRead);
	APEIROGON_API bool				RecvFrom(int8* Data, int32 BufferSize, int32& BytesRead, const IPAddressPtr& ipAddr);
	APEIROGON_API bool				RecvEx(RecvEvent& recvEvnet);

public:
	//SEND
	APEIROGON_API bool				Send(const int8* data, int32 dataLen, int32& BytesSent);
	APEIROGON_API bool				SendTo(const int8* data, int32 dataLen, int32& BytesSent, const IPAddressPtr& ipAddr);
	APEIROGON_API bool				SendEx(SendEvent& sendEvent);

public:
	//ICMP
	APEIROGON_API bool				SendIcmp(IcmpEvent& inIcmpEvent);

public:
	//SOCKET APEIROGON_API OPTIONS
	APEIROGON_API bool				SetBoradcast(bool bAllowBroadcast);
	APEIROGON_API bool				SetIpPktInfo();
	APEIROGON_API bool				SetLinger(bool bShouldLinger, int32 Timeout);
	APEIROGON_API bool				SetNoDelay(bool bIsNoDelay);
	APEIROGON_API bool				SetNonBlocking(bool bIsNonBlocking);
	APEIROGON_API bool				SetReceiveBufferSize(int32 size);
	APEIROGON_API bool				SetReuseAddr(bool bAllowReuse);
	APEIROGON_API bool				SetSendBufferSize(int32 size);
	APEIROGON_API bool				UpdateAcceptSocket(SOCKET listenSocket);
	APEIROGON_API bool				UpdateConnectSocket(bool bIsMaintain);

protected:
	SOCKET							mSocket;
	ESocketType						mSocketType;
	EProtocolType					mProtocolType;
	uint16							mPortNo;
	Icmp							mIcmp;
};