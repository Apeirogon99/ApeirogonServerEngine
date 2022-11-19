#include "pch.h"

int main(void)
{
	setlocale(LC_ALL, "");

	std::wstring ipAddress = L"127.0.0.1";
	uint16 port = 9000;

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	
	WSAStartup(ver, &wsData);

	Socket* sock = SocketUtils::CreateSocket(EProtocolType::IPv4, ESocketType::SOCKTYPE_Streaming);
	
	IPAddress sockAddr;
	sockAddr.SetIp(ipAddress, 9000);

	sock->Connect(sockAddr);

	int8 buf[4096];
	std::string input;
	int32 BytesRead = 0;
	int32 BytesSent = 0;

	do
	{
		std::cout << "> ";
		std::getline(std::cin, input);

		sock->Send(input.c_str(), input.size(), BytesSent);

		ZeroMemory(buf, 4096);
		sock->Recv(buf, 4096, BytesRead);
		std::cout << "SERVER > " << BytesRead << " : " << buf << std::endl;

	} while (BytesRead > 0);

	system("pause");

	SocketUtils::DestroySocket(sock);
	WSACleanup();
}