#include "pch.h"
#include "Socket.h"
#include "SocketTypes.h"

using namespace std;

WinSocket::WinSocket(SOCKET socket, EProtocolType protocolType, ESocketType socketType) : mSocket(socket), mSocketType(socketType), mProtocolType(protocolType), mPortNo(0)
{
  
}

WinSocket::~WinSocket()
{
    //wprintf(L"WinSocket::~WinSocket() : Close window socket\n");

    if (false == Close())
    {
        SocketUtils::WinSocketError(L"~Socket");
    }
}

bool WinSocket::GetConnectionState()
{
    return false;
}

bool WinSocket::GetDescription()
{
    return false;
}

IPAddressPtr WinSocket::GetPeerAddress(const WinSocketPtr& winSocket)
{
    SOCKET socket = winSocket->GetSocket();
    if (socket == INVALID_SOCKET)
    {
        return nullptr;
    }

    int32 sockLen = sizeof(sockaddr_storage);
    sockaddr_storage sockAddr;
    ZeroMemory(&sockAddr, sockLen);
    int32 error = getpeername(socket, reinterpret_cast<SOCKADDR*>(&sockAddr), &sockLen);

    if (SOCKET_ERROR == error)
    {
        SocketUtils::WinSocketError(L"Socket::GetPeerAddress()");
        return nullptr;
    }

    IPAddressPtr ipAddr = std::make_shared<IPAddress>();
    ipAddr->SetIp(sockAddr);

    return ipAddr;
}

uint16 WinSocket::GetPortNo()
{
    return mPortNo;
}

EProtocolType WinSocket::GetProtocolType() const
{
    return mProtocolType;
}

SOCKET WinSocket::GetSocket() const
{
    return mSocket;
}

ESocketType WinSocket::GetSocketType() const
{
    return mSocketType;
}

WinSocket* WinSocket::Accept()
{
    if (mSocket == INVALID_SOCKET)
    {
        return nullptr;
    }

    sockaddr_storage clientAddr;
    int32 addrLen = sizeof(clientAddr);
    SOCKET newSock = INVALID_SOCKET;
    newSock = accept(mSocket, reinterpret_cast<SOCKADDR*>(&clientAddr), &addrLen);
    //::WSAAccept();

    if (newSock == INVALID_SOCKET)
    {
        SocketUtils::WinSocketError(L"Socket::Accept()");
        closesocket(newSock);
        return nullptr;
    }

    WinSocket* clientSock = new WinSocket(newSock, this->mProtocolType, this->mSocketType);
   
    IPAddressPtr clientIPAddress = std::make_shared<IPAddress>();
    clientIPAddress->SetIp(clientAddr);

    return clientSock;
}

bool WinSocket::AcceptEx(const SessionPtr& session, AcceptEvent* acceptEvent)
{
    SOCKET acceptSocket = session->GetWinSocket()->GetSocket();
    RingBuffer& outputBuffer = session->GetRecvBuffer();
    DWORD receiveDataLength = 0;
    DWORD localAddressLength = sizeof(SOCKADDR_IN) + 16;
    DWORD remoteAddressLength = sizeof(SOCKADDR_IN) + 16;
    DWORD bytesReceived = 0;

    bool result = SocketUtils::AcceptEx(
        mSocket, 
        acceptSocket, 
        outputBuffer.GetWriteBuffer(),
        receiveDataLength,
        localAddressLength,
        remoteAddressLength,
        &bytesReceived, 
        static_cast<LPOVERLAPPED>(acceptEvent)
    );

    if (result == false)
    {
        int32 error = WSAGetLastError();
        if (error != WSA_IO_PENDING)
        {
            //SocketUtils::WinSocketError(L"[WinSocket::AcceptEx()]");
            return false;
        }
    }

    return true;
}

bool WinSocket::Bind(IPAddressPtr& ipAddr)
{
    const IPAddress* bindAddr = ipAddr.get();
    if (!bindAddr->IsValid())
    {
        return false;
    }

    if (GetProtocolType() != bindAddr->GetProtocolType())
    {
        return false;
    }

    sockaddr_storage sockAddr = bindAddr->GetSockAddr();
    int32 sockAddrSize = bindAddr->GetAddrSize();
    int32 error = ::bind(mSocket, reinterpret_cast<SOCKADDR*>(&sockAddr), sockAddrSize);


    if (SOCKET_ERROR == error)
    {
        SocketUtils::WinSocketError(L"Socket::Bind()");
        return false;
    }

    return true;
}

bool WinSocket::Close()
{
    if (mSocket != INVALID_SOCKET)
    {
        int32 error = closesocket(mSocket);
        mSocket = INVALID_SOCKET;
        return error != SOCKET_ERROR;
    }

    return true;
}

bool WinSocket::Connect(IPAddressPtr& ipAddr)
{
    IPAddress* connectAddr = ipAddr.get();
    if (!connectAddr->IsValid())
    {
        return false;
    }

    if (GetProtocolType() != connectAddr->GetProtocolType())
    {
        return false;
    }

    int32 sockAddrSize = connectAddr->GetAddrSize();
    sockaddr_storage sockAddr = connectAddr->GetSockAddr();
    int32 error = connect(mSocket, reinterpret_cast<SOCKADDR*>(&sockAddr), sockAddrSize);

    if (SOCKET_ERROR == error)
    {
        SocketUtils::WinSocketError(L"Socket::Connect()");
        return false;
    }

    return true;
}

bool WinSocket::ConnectEx(const IPAddressPtr& ipAddr, ConnectEvent& connectEvent)
{
    if (GetSocketType() != ESocketType::SOCKTYPE_Streaming)
    {
        return false;
    }

    const int32 addrSize = ipAddr->GetAddrSize();
    sockaddr_storage sockAddr = ipAddr->GetSockAddr();
    PVOID sendBuffer = nullptr;
    DWORD sendDataLength = 0;
    DWORD bytesSent = 0;

    bool result = SocketUtils::ConnectEx(mSocket, reinterpret_cast<SOCKADDR*>(&sockAddr), addrSize, sendBuffer, sendDataLength, &bytesSent, &connectEvent);

    if (result == false)
    {
        int32 error = WSAGetLastError();
        if (error != WSA_IO_PENDING)
        {
            SocketUtils::WinSocketError(L"WinSocket::ConnectEx()");
            return false;
        }
    }

    return true;
}

bool WinSocket::DisConnect()
{
    if (false == this->Shutdown())
    {
        return false;
    }

    if (false == this->Close())
    {
        return false;
    }

    return true;
}

bool WinSocket::DisConnectEx(DisconnectEvent& disconnectEvent)
{
    if (GetSocketType() != ESocketType::SOCKTYPE_Streaming)
    {
        return false;
    }

    DWORD flag = TF_REUSE_SOCKET;
    DWORD reserved = 0;
    bool result = SocketUtils::DisconnectEx(mSocket, &disconnectEvent, flag, reserved);

    if (result == false)
    {
        int32 error = WSAGetLastError();
        if (error != WSA_IO_PENDING)
        {
            SocketUtils::WinSocketError(L"WinSocket::DisConnectEx()");
            return false;
        }
    }

    return true;
}

bool WinSocket::Listen(int32 maxBacklog)
{
    if (mSocket == INVALID_SOCKET)
    {
        return false;
    }

    int32 error = listen(mSocket, maxBacklog);

    if (SOCKET_ERROR == error)
    {
        SocketUtils::WinSocketError(L"Socket::Listen()");
        return false;
    }

    return true;
}

bool WinSocket::Shutdown()
{
    bool result = false;
    result = ::shutdown(mSocket, SD_BOTH);

    if (false == result)
    {
        SocketUtils::WinSocketError(L"Socket::Shutdown()");
        return false;
    }

    return true;
}

bool WinSocket::Wait(const int32 waitTime)
{
    bool result = WaitForPendingConnection(waitTime);
    return result == true ? true : false;
}

bool WinSocket::WaitForPendingConnection(const int32 waitTime)
{
    std::this_thread::sleep_for(std::chrono::seconds(waitTime));

    int32 seconds = 0;
    int32 result = setsockopt(mSocket, SOL_SOCKET, SO_CONNECT_TIME, reinterpret_cast<char*>(&seconds), sizeof(seconds));

    if (result != NO_ERROR) {
        SocketUtils::WinSocketError(L"WinSocket::WaitForPendingConnection()");
        return false;
    }
    else
    {
        if (seconds == 0xFFFFFFFF)
        {
            wprintf(L"Connection not established yet\n");
            return false;
        }
        else
        {
            wprintf(L"Connection has been established %ld seconds\n", seconds);
        }
    }

    return true;
}

bool WinSocket::Send(const int8* data, int32 dataLen, int32& BytesSent)
{
    if (mSocket == INVALID_SOCKET)
    {
        return false;
    }

    BytesSent = send(mSocket, data, dataLen, 0);

    if (SOCKET_ERROR == BytesSent)
    {
        SocketUtils::WinSocketError(L"Socket::Send()");
        return false;
    }

    return true;
}

bool WinSocket::SendTo(const int8* data, int32 dataLen, int32& BytesSent, const IPAddressPtr& ipAddr)
{
    if (mSocket == INVALID_SOCKET)
    {
        return false;
    }

    const sockaddr_storage sockAddr = ipAddr->GetSockAddr();
    const int32 toLen = ipAddr->GetAddrSize();
    BytesSent = ::sendto(mSocket, data, dataLen, 0, reinterpret_cast<const SOCKADDR*>(&sockAddr), toLen);

    if (SOCKET_ERROR == BytesSent)
    {
        SocketUtils::WinSocketError(L"Socket::SendTo()");
        return false;
    }

    return true;
}

bool WinSocket::SendEx(SendEvent& sendEvent)
{
    if (mSocket == INVALID_SOCKET)
    {
        return false;
    }

    std::vector<WSABUF> sendBuffer;
    const size_t sendBufferSize = sendEvent.sendBuffers.size();
    sendBuffer.reserve(sendBufferSize);
    for (SendBufferPtr message : sendEvent.sendBuffers)
    {
        WSABUF wsaBuf;
        wsaBuf.buf = reinterpret_cast<char*>(message->Buffer());
        wsaBuf.len = static_cast<LONG>(message->AllocSize());
        sendBuffer.push_back(wsaBuf);
    }

    const DWORD sendBufferCount = static_cast<DWORD>(sendBuffer.size());
    DWORD NumberOfBytesSent = 0;
    const DWORD flag = 0;
    LPWSAOVERLAPPED_COMPLETION_ROUTINE completionRoutine = nullptr;

    int32 WSAResult = ::WSASend(mSocket, sendBuffer.data(), sendBufferCount, &NumberOfBytesSent, flag, &sendEvent, completionRoutine);
    
    if (SOCKET_ERROR == WSAResult)
    {
        int32 error = ::WSAGetLastError();
        if (error != WSA_IO_PENDING)
        {
            SocketUtils::WinSocketError(L"WinSocket::SendEx()");
            return false;
        }
    }

    return true;
}

bool WinSocket::SendIcmp(IcmpEvent& inIcmpEvent)
{
    bool result = mIcmp.SendEcho(inIcmpEvent);
    if (false == result)
    {
        SocketUtils::WinSocketError(L"WinSocket::SendIcmp()");
    }

    return result;
}

bool WinSocket::SetBoradcast(bool bAllowBroadcast)
{
    int Param = bAllowBroadcast ? 1 : 0;

    int32 result = setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&Param), sizeof(Param));

    if (result == SOCKET_ERROR)
    {
        SocketUtils::WinSocketError(L"WinSocket::SetBoradcast()");
        return false;
    }

    return true;
}

bool WinSocket::SetIpPktInfo()
{
    return false;
}

bool WinSocket::SetLinger(bool bShouldLinger, int32 Timeout)
{
    linger ling;
    ling.l_onoff = bShouldLinger;
    ling.l_linger = Timeout;

    int32 result = setsockopt(mSocket, SOL_SOCKET, SO_LINGER, reinterpret_cast<char*>(&ling), sizeof(ling));

    if (result == SOCKET_ERROR)
    {
        SocketUtils::WinSocketError(L"WinSocket::SetLinger()");
        return false;
    }

    return true;
}



bool WinSocket::SetNoDelay(bool bIsNoDelay)
{
    ESocketType curType = GetSocketType();
    if (curType == ESocketType::SOCKTYPE_Streaming)
    {
        int Param = bIsNoDelay ? 1 : 0;

        int32 result = setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&Param), sizeof(Param));

        if (result == SOCKET_ERROR)
        {
            SocketUtils::WinSocketError(L"WinSocket::SetNoDelay()");
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }

    return false;
}

bool WinSocket::SetNonBlocking(bool bIsNonBlocking)
{
    u_long Value = bIsNonBlocking ? true : false;
    int32 result = ::ioctlsocket(mSocket, FIONBIO, &Value);

    if (result == SOCKET_ERROR)
    {
        SocketUtils::WinSocketError(L"WinSocket::SetNonBlocking()");
        return false;
    }

    return true;
}

bool WinSocket::SetReceiveBufferSize(int32 size)
{
    int32 result = setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char*>(&size), sizeof(size));

    if (result == SOCKET_ERROR)
    {
        SocketUtils::WinSocketError(L"WinSocket::SetReceiveBufferSize()");
        return false;
    }

    return true;
}

bool WinSocket::SetReuseAddr(bool bAllowReuse)
{
    int Param = bAllowReuse ? 1 : 0;
    int32 result = setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&bAllowReuse), sizeof(bAllowReuse));

    if (result == SOCKET_ERROR)
    {
        SocketUtils::WinSocketError(L"WinSocket::SetReuseAddr()");
        return false;
    }

    return true;
}

bool WinSocket::SetSendBufferSize(int32 size)
{
    int32 result = setsockopt(mSocket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char*>(&size), sizeof(size));

    if (result == SOCKET_ERROR)
    {
        SocketUtils::WinSocketError(L"WinSocket::SetSendBufferSize()");
        return false;
    }

    return true;
}

bool WinSocket::UpdateAcceptSocket(SOCKET listenSocket)
{
    int32 result = setsockopt(mSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, reinterpret_cast<char*>(&listenSocket), sizeof(listenSocket));

    if (result == SOCKET_ERROR)
    {
        SocketUtils::WinSocketError(L"WinSocket::UpdateAcceptSocket()");
        return false;
    }

    return true;
}

bool WinSocket::UpdateConnectSocket(bool bIsMaintain)
{
    int32 result = setsockopt(mSocket, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, reinterpret_cast<char*>(&bIsMaintain), sizeof(bIsMaintain));

    if (result == SOCKET_ERROR)
    {
        SocketUtils::WinSocketError(L"WinSocket::UpdateConnectSocket()");
        return false;
    }

    return true;
}

bool WinSocket::Recv(int8* data, int32 dataSize, int32& BytesRead)
{
    if (mSocket == INVALID_SOCKET)
    {
        return false;
    }

    BytesRead = recv(mSocket, data, dataSize, 0);

    if (SOCKET_ERROR == BytesRead)
    {
        SocketUtils::WinSocketError(L"Socket::Recv()");
        BytesRead = 0;
        return false;
    }

    return true;
}

bool WinSocket::RecvFrom(int8* Data, int32 BufferSize, int32& BytesRead, const IPAddressPtr& ipAddr)
{
    if (mSocket == INVALID_SOCKET)
    {
        return false;
    }

    sockaddr_storage sockAddr;
    int32 fromLen = sizeof(sockaddr_storage);
    ::memset(&sockAddr, 0, fromLen);

    BytesRead = ::recvfrom(mSocket, Data, BufferSize, 0, reinterpret_cast<SOCKADDR*>(&sockAddr), &fromLen);

    ipAddr->SetIp(sockAddr);

    if (SOCKET_ERROR == BytesRead)
    {
        SocketUtils::WinSocketError(L"Socket::RecvFrom()");
        BytesRead = 0;
        return false;
    }

    return true;
}

bool WinSocket::RecvEx(RecvEvent& recvEvnet)
{
    if (mSocket == INVALID_SOCKET)
    {
        return false;
    }

    WSABUF recvBuffer;
    recvBuffer.buf = reinterpret_cast<int8*>(recvEvnet.mRecvBuffer.GetReadBuffer());
    recvBuffer.len = recvEvnet.mRecvBuffer.GetRecvMaxSize();

    //WSABUF* recvBuffer = buffer;
    const DWORD recvBufferCount = 1;
    DWORD numberOfBytesRecvd = 0;
    DWORD flag = 0;

    int32 WSAResult = ::WSARecv(mSocket, &recvBuffer, recvBufferCount, &numberOfBytesRecvd, &flag, &recvEvnet, nullptr);

    if (SOCKET_ERROR == WSAResult)
    {
        int32 error = ::WSAGetLastError();
        if (error != WSA_IO_PENDING)
        {
            SocketUtils::WinSocketError(L"WinSocket::RecvEx()");
            return false;
        }
    }

    return true;
}
