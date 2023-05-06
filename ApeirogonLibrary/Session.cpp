#include "pch.h"
#include "Session.h"

Session::Session() : mRecvBuffer(0xffff)
{
    mSocket = SocketUtils::CreateSocket(EProtocolType::IPv4, ESocketType::SOCKTYPE_Streaming);
    _InterlockedExchange(&mIsSending, static_cast<LONG>(Default::SESSION_IS_FREE));
}

Session::~Session()
{
    //wprintf(L"[Session::~Session()] : Close session\n");
}

HANDLE Session::GetHandle()
{
    return reinterpret_cast<HANDLE>(mSocket->GetSocket());
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
    EventType currentType = iocpEvent->eventType;
    switch (currentType)
    {
    case EventType::Connect:
        ProcessConnect();
        break;
    case EventType::Disconnect:
        ProcessDisconnect();
        break;
    case EventType::Recv:
        ProcessRecv(numOfBytes);
        break;
    case EventType::Send:
        ProcessSend(numOfBytes);
        break;
    case EventType::Icmp:
        ProcessIcmp();
    default:
        
        break;
    }
}

bool Session::RegisterConnect()
{
    if (true == IsConnected())
    {
        return false;
    }

    if(false == mSocket->SetReuseAddr(true))
    {
        return false;
    }

    IPAddressPtr ipAddr = std::make_shared<IPAddress>();
    ipAddr->SetAnyAddress();
    ipAddr->SetPort(0);

    if (false == mSocket->Bind(ipAddr))
    {
        return false;
    }

    mConnectEvnet.Init();
    mConnectEvnet.owner = shared_from_this();

    mRecvEvent.mRecvBuffer.Clean();


    SessionManagerPtr sessionManager = mSessionManager.lock();
    if (nullptr == sessionManager)
    {
        return false;
    }

    if (false == mSocket->ConnectEx(ipAddr, mConnectEvnet))
    {
        mConnectEvnet.owner = nullptr;
        return false;
    }

    return true;
}

bool Session::RegisterDisconnect()
{   
    mDisconnectEvent.Init();
    mDisconnectEvent.owner = shared_from_this();

    if(false == mSocket->DisConnectEx(mDisconnectEvent))
    {
        mDisconnectEvent.owner = nullptr;
        return false;
    }

    return true;
}

void Session::RegisterRecv()
{
    if (false == IsConnected())
        return;

    mRecvEvent.Init();
    mRecvEvent.owner = shared_from_this();
    //mRecvEvent.Clean();

    if (false == mSocket->RecvEx(mRecvEvent))
    {
        mRecvEvent.owner = nullptr;
    }
}

void Session::RegisterSend()
{

    LONG IsSending = static_cast<LONG>(Default::SESSION_IS_SENDING);
    IsSending = _InterlockedCompareExchange(&mIsSending, static_cast<LONG>(Default::SESSION_IS_SENDING), static_cast<LONG>(Default::SESSION_IS_FREE));

    if (IsSending == static_cast<LONG>(Default::SESSION_IS_SENDING))
    {
        return;
    }

    if (false == IsConnected())
    {
        Disconnect(L"Is not connected");
        return;
    }

    if (mSendQueue.IsEmpty())
    {
        _InterlockedExchange(&mIsSending, static_cast<LONG>(Default::SESSION_IS_FREE));
        return;
    }

    mSendEvent.Init();
    mSendEvent.owner = shared_from_this();
    mSendEvent.Clean();

    // 보낼 데이터를 sendEvent에 등록
    mSendQueue.Pop(mSendEvent.sendBuffers);

    if (false == mSocket->SendEx(mSendEvent))
    {
        mSendEvent.owner = nullptr;
        mSendEvent.Clean();
        _InterlockedExchange(&mIsSending, static_cast<LONG>(Default::SESSION_IS_FREE));
    }
    
}

void Session::RegisterIcmp()
{

    LONG IsSending = static_cast<LONG>(Default::SESSION_IS_SENDING);
    IsSending = _InterlockedCompareExchange(&mIsSending, static_cast<LONG>(Default::SESSION_IS_SENDING), static_cast<LONG>(Default::SESSION_IS_FREE));

    if (IsSending == static_cast<LONG>(Default::SESSION_IS_SENDING))
    {
        return;
    }

    if (false == IsConnected())
    {
        Disconnect(L"Is not connected");
        return;
    }

    mIcmpEvent.Init();
    mIcmpEvent.Clean();
    mIcmpEvent.owner = shared_from_this();
    mIcmpEvent.mIpAddr = mIpAddr;
    mIcmpEvent.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (false == mSocket->SendIcmp(mIcmpEvent))
    {
        mIcmpEvent.owner = nullptr;
        mIcmpEvent.Clean();
        _InterlockedExchange(&mIsSending, static_cast<LONG>(Default::SESSION_IS_FREE));
    }
    else
    {
        GetSessionManager()->GetService()->GetIOCPServer()->PostDispatch(0, mIcmpEvent);
    }

}

void Session::ProcessConnect()
{
    mConnectEvnet.owner = nullptr;
    mIsConnect.store(true);

    //if (false == mSocket->UpdateConnectSocket(true))
    //{
    //    Disconnect(L"Failed to UpdateConnectSocket");
    //}

    SessionManagerPtr manager = mSessionManager.lock();
    if (nullptr == manager)
    {
        Disconnect(L"Invalid session manager");
        return;
    }

    if (false == manager->InsertSession(GetSession()))
    {
        Disconnect(L"Failed to insert connect session");
        return;
    }

    OnConnected();
    
    RegisterRecv();
}

void Session::ProcessDisconnect()
{
    mDisconnectEvent.owner = nullptr;

    SessionManagerPtr manager = mSessionManager.lock();
    if (nullptr == manager)
    {
        return;
    }

    if (false == manager->ReleaseSession(GetSession()))
    {
        SessionLog(L"Session::ProcessDisconnect()");
        return;
    }

    OnDisconnected();
}

void Session::ProcessRecv(const uint32 numOfBytes)
{
    mRecvEvent.owner = nullptr;

    if (numOfBytes == 0)
    {
        Disconnect(L"Recv 0");
        return;
    }

    const uint32 stored = mRecvBuffer.Enqueue(mRecvEvent.mRecvBuffer.GetReadBuffer(), numOfBytes);
    mRecvEvent.mRecvBuffer.MoveFront(stored);
    if (stored == 0)
    {
        Disconnect(L"Free Overflow");
        return;
    }

    const uint32 dataSize = mRecvBuffer.GetUsedSize();
    uint32 processLen = OnRecv(mRecvBuffer, dataSize);

    if (processLen == 0)
    {
        Disconnect(L"OnRecv Overflow");
        return;
    }

    RegisterRecv();
}

void Session::ProcessSend(const uint32 numOfBytes)
{
    mSendEvent.owner = nullptr;
    mSendEvent.Clean();

    if (numOfBytes == 0)
    {
        Disconnect(L"Send 0");
        return;
    }

    OnSend(numOfBytes);

    if (mSendQueue.IsEmpty())
    {
        _InterlockedExchange(&mIsSending, static_cast<LONG>(Default::SESSION_IS_FREE));
    }
    else
    {
        RegisterSend();
    }
}

void Session::ProcessIcmp()
{

    PICMP_ECHO_REPLY echoReply = reinterpret_cast<PICMP_ECHO_REPLY>(mIcmpEvent.mReplyBuffer.GetReadBuffer());
    if (echoReply->Address != 0 && echoReply->Status == IP_SUCCESS)
    {        
        const int64 limitRTT = 50;
        const int64 avgRTT = mRoundTripTime.GetRoundTripTime();
        const int64 roundTripTime = echoReply->RoundTripTime;

        if (roundTripTime > avgRTT + limitRTT)
        {
            return;
        }

        mRoundTripTime.AddLatency(roundTripTime);
        wprintf(L"ADDR = %ld, RTT = %lld\n", echoReply->Address ,echoReply->RoundTripTime);
    }

    _InterlockedExchange(&mIsSending, static_cast<LONG>(Default::SESSION_IS_FREE));

}

void Session::Connect()
{
    RegisterConnect();
}

void Session::Disconnect(const WCHAR* cause)
{
    if (mIsConnect.exchange(false) == false)
        return;

    SessionLog(L"[Session::Disconnect()] : %ws\n", cause);

    RegisterDisconnect();
}

void Session::Send(SendBufferPtr sendBuffer)
{
    if (IsConnected() == false)
    {
        Disconnect(L"Can't Send");
        return;
    }

    mSendQueue.Push(sendBuffer);
}

void Session::Recv()
{
    if (IsConnected() == false)
        return;
}

void Session::SetSessionManager(const SessionManagerRef& sessionManager)
{
    mSessionManager = sessionManager;
}

void Session::SetIpAddress(IPAddressPtr& ipAddr)
{
    mIpAddr = std::move(ipAddr);
}

void Session::Shutdown()
{
    mSocket.reset();
    mIpAddr.reset();
    mSessionManager.reset();
}

bool Session::IsValid()
{
    SessionManagerPtr manager =  mSessionManager.lock();
    SessionPtr session = std::static_pointer_cast<Session>(shared_from_this());
    bool valid = manager->FindSession(session);
    return valid;
}

void Session::SessionLog(const WCHAR* log , ...)
{
    SessionManagerPtr manager = mSessionManager.lock();
    if (nullptr == manager)
    {
        return;
    }

    manager->SessionManagerLog(log);
    //mService->GetLoggerManager()->WriteLog(log);
}

bool Session::IsConnected() const
{
    return mIsConnect;
}

SessionPtr Session::GetSession()
{
    return std::static_pointer_cast<Session>(shared_from_this());
}

WinSocketPtr Session::GetWinSocket() const
{
    return mSocket;
}

IPAddressPtr Session::GetIpAddress() const
{
    return mIpAddr;
}

RingBuffer& Session::GetRecvBuffer()
{
    return mRecvBuffer;
}

const int64 Session::GetRoundTripTime()
{
    const int64 avgRTT = mRoundTripTime.GetRoundTripTime();
    const int64 halfRTT = avgRTT / 2;
    return halfRTT;
}

SessionManagerPtr Session::GetSessionManager()
{
    SessionManagerPtr manager = mSessionManager.lock();
    
    if (nullptr == manager)
    {
        return nullptr;
    }

    return manager;
}
