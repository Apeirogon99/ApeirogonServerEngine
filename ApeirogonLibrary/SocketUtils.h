#pragma once

#ifdef DLL_DEVELOPMENT
#define SOCKET_API __declspec(dllimport)
#else
#define SOCKET_API __declspec(dllexport)
#endif // DLL_DEVELOPMENT

class Socket;
class SOCKET_API SocketUtils
{
public:

	static Socket*	CreateSocket(EProtocolType protocolType, ESocketType socketType);

	static void		DestroySocket(Socket* sock);

	static void		PrintSocketError(const WCHAR* function);

	static WCHAR*	GetSocektError();

};