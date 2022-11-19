#include "pch.h"
#include "Socket.h"
#include "SocketUtils.h"

class Socket;

int main(void)
{
	setlocale(LC_ALL, "");

	WSADATA data;
	WORD  ver = MAKEWORD(2, 2);

	WSAStartup(ver, &data);

	Socket* listening = SocketUtils::CreateSocket(EProtocolType::IPv4, ESocketType::SOCKTYPE_Streaming);

	IPAddress listeningAddr;
	listeningAddr.SetIp(L"127.0.0.1", 9000);

	listening->Bind(listeningAddr);

	listening->Listen();

	Socket* clientSocket = listening->Accept();

	std::wcout << L"[CONNECT]" << clientSocket->GetAddress().ToString() << std::endl;

	SocketUtils::DestroySocket(listening);

	int8 buf[4096];
	int32 BytesRead;
	int32 BytesSent;

	while (clientSocket)
	{
		ZeroMemory(buf, 4096);

		clientSocket->Recv(buf, 4096, BytesRead);
		if (BytesRead == 0)
		{
			std::cout << "END" << std::endl;
			break;
		}

		std::cout << "CLIENT > " << BytesRead << " : " << buf << std::endl;

		clientSocket->Send(buf, BytesRead, BytesSent);
	}

	system("pause");

	SocketUtils::DestroySocket(clientSocket);
	WSACleanup();
}