#include "pch.h"
#include "Socket.h"
#include "SocketTypes.h"

using namespace std;

Socket::Socket(SOCKET socket, EProtocolType protocolType, ESocketType socketType) : mSocket(socket), mSocketType(socketType), mProtocolType(protocolType)
{
    mPortNo = 0;
}

Socket::~Socket()
{
    if (false == Close())
    {
        SocketUtils::PrintSocketError(L"~Socket");
    }
}

void Socket::SetAddress(IPAddress& ipAddr)
{
    mIpAddress = ipAddr;
}

IPAddress Socket::GetAddress()
{
    return mIpAddress;
}

bool Socket::GetConnectionState()
{
    return false;
}

bool Socket::GetDescription()
{
    return false;
}

bool Socket::GetPeerAddress()
{
    if (mSocket == INVALID_SOCKET)
    {
        return false;
    }

    if (!mIpAddress.IsValid())
    {
        return false;
    }

    int32 sockLen = mIpAddress.GetAddrSize();
    int32 error = getpeername(mSocket, reinterpret_cast<SOCKADDR*>(&mIpAddress.GetSockAddr()), &sockLen);

    if (SOCKET_ERROR == error)
    {
        SocketUtils::PrintSocketError(L"Socket::GetPeerAddress()");
        return false;
    }


    return true;
}

uint16 Socket::GetPortNo()
{
    return mPortNo;
}

EProtocolType Socket::GetProtocolType() const
{
    return mProtocolType;
}

SOCKET Socket::GetSocket() const
{
    return mSocket;
}

ESocketType Socket::GetSocketType() const
{
    return mSocketType;
}

Socket* Socket::Accept()
{
    if (mSocket == INVALID_SOCKET)
    {
        return nullptr;
    }

    SOCKADDR_IN clientAddr;
    int32 addrLen = sizeof(clientAddr);
    SOCKET newSock = INVALID_SOCKET;
    newSock = accept(mSocket, (SOCKADDR*)&clientAddr, &addrLen);

    if (newSock == INVALID_SOCKET)
    {
        SocketUtils::PrintSocketError(L"Socket::Accept()");
        closesocket(newSock);
        return nullptr;
    }

    Socket* clientSock = new Socket(newSock, this->mProtocolType, this->mSocketType);
    IPAddress clientIPAddress(clientAddr);
    clientSock->SetAddress(clientIPAddress);

    return clientSock;
}

bool Socket::Bind(IPAddress& ipAddr)
{
    if (mSocket == INVALID_SOCKET)
    {
        return false;
    }

    if (!ipAddr.IsValid())
    {
        return false;
    }

    if (GetProtocolType() != ipAddr.GetProtocolType())
    {
        return false;
    }

    SetAddress(ipAddr);
    if (!mIpAddress.IsValid())
    {
        return false;
    }

    int32 error = ::bind(mSocket, (sockaddr*)&mIpAddress.GetSockAddr(), mIpAddress.GetAddrSize());

    if (SOCKET_ERROR == error)
    {
        SocketUtils::PrintSocketError(L"Socket::Bind()");
        return false;
    }

    return true;
}

bool Socket::Close()
{
    if (mSocket != INVALID_SOCKET)
    {
        int32 error = closesocket(mSocket);
        mSocket = INVALID_SOCKET;
        return error != SOCKET_ERROR;
    }
    return false;
}

bool Socket::Connect(IPAddress& ipAddr)
{
    if (mSocket == INVALID_SOCKET)
    {
        return false;
    }

    if (!ipAddr.IsValid())
    {
        return false;
    }

    if (GetProtocolType() != ipAddr.GetProtocolType())
    {
        return false;
    }

    SetAddress(ipAddr);
    if (!mIpAddress.IsValid())
    {
        return false;
    }

    int32 error = connect(mSocket, (sockaddr*)&mIpAddress, mIpAddress.GetAddrSize());

    if (SOCKET_ERROR == error)
    {
        SocketUtils::PrintSocketError(L"Socket::Connect()");
        return false;
    }

    return true;
}

bool Socket::Listen(int32 maxBacklog)
{
    if (mSocket == INVALID_SOCKET)
    {
        return false;
    }

    int32 error = listen(mSocket, maxBacklog);

    if (SOCKET_ERROR == error)
    {
        SocketUtils::PrintSocketError(L"Socket::Listen()");
        return false;
    }

    return true;
}

bool Socket::Send(const int8* data, int32 dataLen, int32& BytesSent)
{
    if (mSocket == INVALID_SOCKET)
    {
        return false;
    }

    BytesSent = send(mSocket, data, dataLen, 0);

    if (SOCKET_ERROR == BytesSent)
    {
        SocketUtils::PrintSocketError(L"Socket::Send()");
        return false;
    }

    return true;
}

bool Socket::SetBoradcast(bool bAllowBroadcast)
{
    int Param = bAllowBroadcast ? 1 : 0;
    return SetSocketOption(SOL_SOCKET, SO_BROADCAST, Param);
}

bool Socket::SetIpPktInfo()
{
    return false;
}

bool Socket::SetLinger(bool bShouldLinger, int32 Timeout)
{
    linger ling;
    ling.l_onoff = bShouldLinger;
    ling.l_linger = Timeout;

    return SetSocketOption(SOL_SOCKET, SO_LINGER, ling);
}

bool Socket::SetMulticatInterface()
{
    return false;
}

bool Socket::SetMulticastLoopBack()
{
    return false;
}

bool Socket::SetMulticatTtl()
{
    return false;
}

bool Socket::SetNoDelay(bool bIsNoDelay)
{
    if (GetSocketType() == ESocketType::SOCKTYPE_Streaming)
    {
        int Param = bIsNoDelay ? 1 : 0;
        return SetSocketOption(IPPROTO_TCP, TCP_NODELAY, Param);
    }

    return true;
}

bool Socket::SetNonBlocking(bool bIsNonBlocking)
{
    u_long Value = bIsNonBlocking ? true : false;
    return SOCKET_ERROR != ::ioctlsocket(mSocket, FIONBIO, &Value);
}

bool Socket::SetReceiveBufferSize(int32 size)
{
    return SetSocketOption(SOL_SOCKET, SO_RCVBUF, size);
}

bool Socket::SetRecvErr()
{
    return false;
}

bool Socket::SetRetrieveTimestamp()
{
    return false;
}

bool Socket::SetReuseAddr(bool bAllowReuse)
{
    int Param = bAllowReuse ? 1 : 0;
    return SetSocketOption(SOL_SOCKET, SO_REUSEADDR, Param);
}

bool Socket::SetSendBufferSize(int32 size)
{
    return SetSocketOption(SOL_SOCKET, SO_SNDBUF, size);
}

bool Socket::UpdateAcceptSocket(SOCKET listenSocket)
{
    return SetSocketOption(SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
}

template<typename T>
bool Socket::SetSocketOption(int32 level, int32 optName, T optVal)
{
    return SOCKET_ERROR != ::setsockopt(mSocket, level, optName, reinterpret_cast<char*>(&optVal), sizeof(T));
}

bool Socket::Recv(int8* data, int32 dataSize, int32& BytesRead)
{
    if (mSocket == INVALID_SOCKET)
    {
        return false;
    }

    BytesRead = recv(mSocket, data, dataSize, 0);

    if (SOCKET_ERROR == BytesRead)
    {
        SocketUtils::PrintSocketError(L"Socket::Recv()");
        BytesRead = 0;
        return false;
    }

    return true;
}
