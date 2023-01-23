#pragma once

class WinSocket;
class IPAddress;

class SocketUtils
{
public:
	static LPFN_CONNECTEX		ConnectEx;
	static LPFN_DISCONNECTEX	DisconnectEx;
	static LPFN_ACCEPTEX		AcceptEx;

public:
	SOCKET_API static void			Init();
	SOCKET_API static void			Clear();

	SOCKET_API static bool			BindWindowsFunction(WinSocketPtr socket, GUID guid, LPVOID* fn);

	SOCKET_API static WinSocketPtr	CreateSocket(EProtocolType protocolType, ESocketType socketType);

	SOCKET_API static void			DestroySocket(WinSocketPtr sock);

	SOCKET_API static void			WinSocketError(const WCHAR* function);

	SOCKET_API static WCHAR*		GetSocektError();

};