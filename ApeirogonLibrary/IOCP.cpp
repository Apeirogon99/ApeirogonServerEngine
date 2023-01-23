#include "pch.h"
#include "IOCP.h"

IOCPServer::IOCPServer()
{
	
}

IOCPServer::~IOCPServer()
{
	::CloseHandle(mIOCPHandle);
	wprintf(L"IOCPServer::~IOCPServer()\n");
}

bool IOCPServer::Prepare(const ServicePtr& service)
{
	mService = service;
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

	return true;
}

void IOCPServer::Shutdown()
{
	mService.reset();
	wprintf(L"IOCPServer::Shutdown() : iocp server successfully shutdown\n");
}

bool IOCPServer::RegisterSocketToIOCP(const WinSocketPtr sock)
{
	HANDLE result;
	HANDLE registerSocket = reinterpret_cast<HANDLE>(sock->GetSocket());
	result = ::CreateIoCompletionPort(registerSocket, mIOCPHandle, NULL, NULL);

	if (result == NULL)
	{
		IOCPServerLog(L"IOCP::RegisterSocketToIOCP()");
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
		IOCPServerLog(L"IOCPServer::WorkDispatch()");
		return false;
	}

	IOCPObjectPtr iocpObject = iocpEvent->owner;
	iocpObject->Dispatch(iocpEvent, numOfBytes);

	return true;
}

bool IOCPServer::PostDispatch(const uint32 len, const ULONG_PTR key)
{
	const DWORD numOfBytes = static_cast<DWORD>(len);
	IocpEvent* iocpEvent = nullptr;

	bool ret = ::PostQueuedCompletionStatus(mIOCPHandle, numOfBytes, key, iocpEvent);
	if (ret == false)
	{
		IOCPServerLog(L"IOCPServer::PostDispatch()");
		return false;
	}

	return true;
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