#include "pch.h"
#include "SocketUtils.h"

using namespace std;

Socket* SocketUtils::CreateSocket(EProtocolType protocolType, ESocketType socketType)
{
	SOCKET sock = INVALID_SOCKET;
	Socket* newSock = nullptr;

	if (protocolType == EProtocolType::None)
	{
		protocolType = EProtocolType::IPv4;
	}

	int32 af = static_cast<int32>(protocolType);
	int32 type = static_cast<int32>(socketType);
	int32 protocol = (socketType == ESocketType::SOCKTYPE_Streaming) ? IPPROTO_TCP : IPPROTO_UDP;

	sock = ::WSASocket(af, type, protocol, NULL, 0, WSA_FLAG_OVERLAPPED);
	//sock = socket(af, type, protocol);

	if (sock == INVALID_SOCKET)
	{
		PrintSocketError(L"CreateSocket");
		closesocket(sock);
	}
	else
	{
		newSock = new Socket(sock, protocolType, socketType);
	}

	if (!newSock)
	{
		printf("[CreateSocket] : Failed to create socket\n");
	}

	return newSock;
}

void SocketUtils::DestroySocket(Socket* sock)
{
	if (sock)
	{
		delete sock;
	}
	
	sock = nullptr;
}

void SocketUtils::PrintSocketError(const WCHAR* function)
{
	int32 code = WSAGetLastError();
	if(code == WSA_IO_PENDING)
	{
		return;
	}

	wprintf(L"[%s] ERROR(%d) : %s", function, code, GetSocektError());
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
		WSAGetLastError(), 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(LPWSTR)&pBuffer,
		0, 
		NULL);

	return pBuffer;
}
