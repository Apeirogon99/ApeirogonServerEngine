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
	APEIROGON_API static void			Init();
	APEIROGON_API static void			Clear();

	APEIROGON_API static bool			BindWindowsFunction(WinSocketPtr socket, GUID guid, LPVOID* fn);

	APEIROGON_API static WinSocketPtr	CreateSocket(EProtocolType protocolType, ESocketType socketType);

	APEIROGON_API static void			DestroySocket(WinSocketPtr sock);

	APEIROGON_API static void			WinSocketError(const WCHAR* function);

	APEIROGON_API static WCHAR*		GetSocektError();

};