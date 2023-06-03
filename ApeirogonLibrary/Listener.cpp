#include "pch.h"
#include "Listener.h"

Listener::Listener(IPAddressPtr& ipAddr) : mListenSocket(nullptr), mIpAddr(std::move(ipAddr))
{
	
}

Listener::~Listener()
{
	wprintf(L"[Listener::~Listener()]\n");
}

bool Listener::Prepare(ServicePtr service)
{
	this->mService = service;
	if (nullptr == mService)
	{
		return false;
	}

	if (false == mIpAddr->IsValid())
	{
		return false;
	}
	
	mListenSocket = SocketUtils::CreateSocket(EProtocolType::IPv4, ESocketType::SOCKTYPE_Streaming);
	if (nullptr == mListenSocket)
	{
		return false;
	}

	if (false == mService->GetIOCPServer()->RegisterSocketToIOCP(mListenSocket))
	{
		return false;
	}

	if (false == mListenSocket->SetReuseAddr(true))
	{
		return false;
	}

	if (false == mListenSocket->SetLinger(false, 0))
	{
		return false;
	}

	if (false == mListenSocket->Bind(mIpAddr))
	{
		return false;
	}

	if (false == mListenSocket->Listen())
	{
		return false;
	}

	const uint32 acceptCount = mService->GetSessionManager()->GetMaxSessionCount();
	for (uint32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = new AcceptEvent();
		acceptEvent->owner = shared_from_this();
		mAcceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}

	ListenerLog(L"[Listener::Prepare()] Sessions accept ready\n", acceptCount);

	return true;
}

void Listener::Shutdown()
{
	const uint64 acceptSize = mAcceptEvents.size();
	for (AcceptEvent* acceptEvent : mAcceptEvents)
	{
		acceptEvent->owner.reset();
		delete acceptEvent;
		acceptEvent = nullptr;
	}

	ListenerLog(L"[Listener::Shutdown()] Listener successfully shutdown\n");

	mService.reset();
	mIpAddr.reset();
	mListenSocket.reset();
}

void Listener::CloseSocket()
{
	SocketUtils::DestroySocket(mListenSocket);
}

WinSocketPtr Listener::GetListenerSocket()
{
	return mListenSocket;
}

IPAddressPtr Listener::GetIpAddress()
{
	return mIpAddr;
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(mListenSocket->GetSocket());
}

void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	if (iocpEvent->eventType != EventType::Accept)
	{
		return;
	}

	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	SessionPtr newSession = mService->GetSessionManager()->CreateSession();

	acceptEvent->Init();
	acceptEvent->session = newSession;

	DWORD bytesReceived = 0;

	bool result = mListenSocket->AcceptEx(newSession, acceptEvent);
	if (false == result)
	{
		int32 error = WSAGetLastError();
		ListenerLog(L"[Listener::RegisterAccept] (%d)", error);

		RegisterAccept(acceptEvent);

		return;
	}
}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	SessionPtr session = acceptEvent->session;

	WinSocketPtr clientSocket = session->GetWinSocket();
	if (false == clientSocket->UpdateAcceptSocket(mListenSocket->GetSocket()))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	IPAddressPtr clientIpAddr = mListenSocket->GetPeerAddress(clientSocket);
	if (nullptr == clientIpAddr)
	{
		RegisterAccept(acceptEvent);
		return;
	}

	ListenerLog(L"[CLIENT ACCEPT] %ws\n", clientIpAddr->ToString().c_str());

	session->SetIpAddress(clientIpAddr);
	session->ProcessConnect();
	RegisterAccept(acceptEvent);
}

void Listener::ListenerLog(const WCHAR* log, ...)
{
	mService->GetLoggerManager()->WriteLog(log);
}