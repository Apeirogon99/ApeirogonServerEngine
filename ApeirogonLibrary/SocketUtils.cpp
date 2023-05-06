#include "pch.h"
#include "SocketUtils.h"

using namespace std;

LPFN_CONNECTEX		SocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX	SocketUtils::DisconnectEx = nullptr;
LPFN_ACCEPTEX		SocketUtils::AcceptEx = nullptr;

void SocketUtils::Init()
{
	int32 result = 0;
	WSADATA data;
	WORD	ver = MAKEWORD(2, 2);

	result = ::WSAStartup(ver, &data);
	if (SOCKET_ERROR == result)
	{
		return;
	}

	HINSTANCE IcmpDll = LoadLibrary(L"ICMP.DLL");
	if (IcmpDll == NULL)
	{
		return;
	}



	WinSocketPtr dummySocket = CreateSocket(EProtocolType::IPv4, ESocketType::SOCKTYPE_Streaming);

	BindWindowsFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx));
	BindWindowsFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx));
	BindWindowsFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx));

	DestroySocket(dummySocket);
}

void SocketUtils::Clear()
{
	WSACleanup();
}

bool SocketUtils::BindWindowsFunction(WinSocketPtr socket, GUID guid, LPVOID* fn)
{
	int32 result = 0;
	DWORD bytes = 0;
	result = ::WSAIoctl(socket->GetSocket(), SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);

	if (SOCKET_ERROR == result)
	{
		WinSocketError(L"SocketUtils::BindWindowsFunction()");
		return false;
	}

	return true;
}

WinSocketPtr SocketUtils::CreateSocket(EProtocolType protocolType, ESocketType socketType)
{
	SOCKET sock = INVALID_SOCKET;
	WinSocketPtr newWinSock = nullptr;

	if (protocolType == EProtocolType::None)
	{
		return nullptr;
	}

	int32 family = static_cast<int32>(protocolType);
	int32 type = static_cast<int32>(socketType);
	int32 protocol = (socketType == ESocketType::SOCKTYPE_Streaming) ? IPPROTO_TCP : IPPROTO_UDP;

	sock = ::WSASocket(family, type, protocol, NULL, 0, WSA_FLAG_OVERLAPPED);
	//sock = socket(af, type, protocol);

	if (sock == INVALID_SOCKET)
	{
		WinSocketError(L"CreateSocket");
		closesocket(sock);
		return nullptr;
	}

	newWinSock = std::make_shared<WinSocket>(sock, protocolType, socketType);
	//newSock = new WinSocket(sock, protocolType, socketType);
	if (nullptr == newWinSock)
	{
		printf("[CreateSocket] : Failed to create socket\n");
	}

	return newWinSock;
}

void SocketUtils::DestroySocket(WinSocketPtr sock)
{
	if (sock)
	{
		//sock->Close();
		sock.reset();
	}
}

void SocketUtils::WinSocketError(const WCHAR* function)
{
	//후에는 로그로 가야함

	int32 code = WSAGetLastError();
	if(code == WSA_IO_PENDING)
	{
		return;
	}

	wprintf(L"[%ws][%ws] ERROR(%d) : %ws", Time::NowTime(true).c_str(), function, code, GetSocektError());
}

WCHAR* SocketUtils::GetSocektError()
{
	//int32 code = WSAGetLastError();
	//::wstring message;
	//switch (code)
	//{
	//case WSA_INVALID_HANDLE: message = L"";
	//default:
	//	break;
	//}

	LPWSTR pBuffer = NULL;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, 
		GetLastError(), 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(LPWSTR)&pBuffer,
		0, 
		NULL);

	return pBuffer;
}
