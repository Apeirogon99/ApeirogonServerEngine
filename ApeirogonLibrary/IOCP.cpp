#include "pch.h"
#include "IOCP.h"

IOCPServer::IOCPServer() : mIOCPHandle(INVALID_HANDLE_VALUE)
{
	
}

IOCPServer::~IOCPServer()
{
	::CloseHandle(mIOCPHandle);
	wprintf(L"[IOCPServer::~IOCPServer()]\n");
}

bool IOCPServer::Prepare(ServicePtr service)
{
	this->mService = service;
	if (nullptr == mService)
	{
		return false;
	}

	mIOCPHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	
	if (mIOCPHandle == INVALID_HANDLE_VALUE)
	{
		IOCPServerLog(L"IOCP::Init()");
		return false;
	}



	IOCPServerLog(L"[IOCPServer::Prepare()] Server is running");

	return true;
}

void IOCPServer::Shutdown()
{
	IOCPServerLog(L"[IOCPServer::Shutdown()] Iocp server successfully shutdown ");
	mService.reset();
}

bool IOCPServer::RegisterSocketToIOCP(const WinSocketPtr sock)
{
	HANDLE result;
	HANDLE registerSocket = reinterpret_cast<HANDLE>(sock->GetSocket());
	result = ::CreateIoCompletionPort(registerSocket, mIOCPHandle, NULL, NULL);

	if (result == NULL)
	{
		IOCPServerLog(L"[IOCP::RegisterSocketToIOCP()]");
		return false;
	}

	return true;
}

bool IOCPServer::WorkDispatch(uint32 timeoutMs)
{
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0;
	IocpEvent* iocpEvent = nullptr;

	bool ret = ::GetQueuedCompletionStatus(mIOCPHandle, &numOfBytes, &key, reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs);
	
	if(ret == false)
	{
		IOCPServerLog(L"[IOCPServer::WorkDispatch()]");
		IOCPErrorHandling(iocpEvent);
		return false;
	}

	IOCPObjectPtr iocpObject = iocpEvent->owner;
	iocpObject->Dispatch(iocpEvent, numOfBytes);

	return true;
}

bool IOCPServer::PostDispatch(const uint32 inNumOfBytes, IocpEvent& inEvent)
{
	ULONG_PTR key = 0;

	bool ret = ::PostQueuedCompletionStatus(mIOCPHandle, inNumOfBytes, key, &inEvent);
	if (ret == false)
	{
		IOCPServerLog(L"[IOCPServer::PostDispatch()]");
		return false;
	}

	return true;
}

void IOCPServer::IOCPErrorHandling(IocpEvent* inEvent)
{
	int32 code = WSAGetLastError();

	switch (code)
	{
	case WSA_WAIT_TIMEOUT: return;
	case WSA_IO_PENDING: return;
	}

	IocpObject* object = inEvent->owner.get();
	if (nullptr == object)
	{
		mService->GetLoggerManager()->WriteLog(L"[IOCPServer::IOCPErrorHandling] IocpObject is nullptr");
		return;
	}

	SessionPtr session = std::static_pointer_cast<Session>(inEvent->owner);
	switch (code)
	{
	case ERROR_NETNAME_DELETED:
		session->Disconnect(L"ERROR_NETNAME_DELETED");
		break;

	case ERROR_NETWORK_ACCESS_DENIED:
		session->Disconnect(L"ERROR_NETWORK_ACCESS_DENIED");
		break;

	default:
		mService->GetLoggerManager()->WriteLog(L"[IOCPServer::IOCPErrorHandling] Can't Handling Error");
		break;
	}
}

void IOCPServer::IOCPServerLog(const WCHAR* func)
{
	int32 code = WSAGetLastError();
	if (code == WSA_IO_PENDING || code == WSA_WAIT_TIMEOUT)
	{
		return;
	}

	WCHAR* Buffer = nullptr;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(WCHAR*)&Buffer,
		0,
		nullptr);

	std::wstring log;
	log.append(func);
	log.append(L" Error(");
	log.append(std::to_wstring(code));
	log.append(L") : ");
	log.append(Buffer);

	mService->GetLoggerManager()->WriteLog(log.c_str());

	LocalFree(Buffer);
}