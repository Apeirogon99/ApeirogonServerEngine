#include "pch.h"
#include "IdentityPacket.pb.h"
#include "IdentityClientPacketHandler.h"

static int32 packetHeaderSize = static_cast<int32>(sizeof(PacketHeader));

WinSocketPtr StartUp()
{
	wprintf(L"DUMMY CLIENT\n");
	const WCHAR* ipAddress = L"127.0.0.1";

	uint16 port = 9000;

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	WSAStartup(ver, &wsData);

	WinSocketPtr sock = SocketUtils::CreateSocket(EProtocolType::IPv4, ESocketType::SOCKTYPE_Streaming);

	IPAddressPtr sockAddr = std::make_shared<IPAddress>();
	//sockAddr->SetIp(ipAddress, 9000, EProtocolType::IPv4);
	sockAddr->SetLoopbackAddress();
	sockAddr->SetPort(9000);

	std::wcout << "IP : " << sockAddr->GetIp() << std::endl;
	std::wcout << "PORT : " << sockAddr->GetPort() << std::endl;

	if (false == sock->Connect(sockAddr))
	{
		printf("NOT CONNECT\n");
	}
	else
	{
		printf("CONNECT\n");
	}

	return sock;
}

void ENCODE_SINGING(WinSocketPtr sock)
{
	Protocol::C2S_Singin C2S_pktSingin;
	C2S_pktSingin.set_id("TEST_0003");
	C2S_pktSingin.set_password("1234");

	const uint16 dataSize = static_cast<uint16>(C2S_pktSingin.ByteSizeLong());
	const uint16 packetSize = static_cast<uint16>(dataSize + packetHeaderSize);

	PacketHeader header;
	header.id = static_cast<uint32>(EPakcetID::C2S_Singin);
	header.size = packetSize;

	char* sendBuf = new char[packetSize];
	ZeroMemory(sendBuf, packetSize);

	memcpy(sendBuf, &header, packetHeaderSize);
	C2S_pktSingin.SerializeToArray(sendBuf + packetHeaderSize, dataSize);

	int32 BytesSent = 0;
	sock->Send(sendBuf, (dataSize + packetHeaderSize), BytesSent);

	if (BytesSent == -1)
	{
		printf("SEND 0\n");
		exit(0);
	}
	

	printf("Client Send : [%d (Byte)] [ID : %s][PS : %s]\n", BytesSent, C2S_pktSingin.id().c_str(), C2S_pktSingin.password().c_str());

	//IPAddressPtr ipAddr = std::make_shared<IPAddress>();
	//ipAddr->SetLoopbackAddress();
	//ipAddr->SetPort(9000);

	//sock->Send(sendBuf, (dataSize + packetHeaderSize), BytesSent);
}

void DECODE_SINGIN(WinSocketPtr sock)
{
	int8 Recvbuf[4096];
	int32 BytesRead = 0;

	ZeroMemory(Recvbuf, 4096);
	sock->Recv(Recvbuf, 4096, BytesRead);

	//IPAddressPtr ipAddr = std::make_shared<IPAddress>();
	//sock->Recv(Recvbuf, 4096, BytesRead);

	if (BytesRead == 0)
	{
		printf("Read 0\n");
		exit(0);
	}

	PacketHeader headerRecv;
	memcpy(&headerRecv, Recvbuf, packetHeaderSize);

	Protocol::S2C_Singin S2C_SinginRECV;
	S2C_SinginRECV.ParseFromArray(Recvbuf + packetHeaderSize, headerRecv.size - packetHeaderSize);

	printf("Client Recv : [%d (Byte)] [TOKEN : %s]\n", BytesRead, S2C_SinginRECV.id_token().c_str());
}

//void UDP()
//{
//	wprintf(L"DUMMY CLIENT\n");
//	const WCHAR* ipAddress = L"127.0.0.1";
//
//	uint16 port = 9000;
//
//	WSADATA wsData;
//	WORD ver = MAKEWORD(2, 2);
//
//	WSAStartup(ver, &wsData);
//
//	WinSocket* sock = SocketUtils::CreateSocket(EProtocolType::IPv4, ESocketType::SOCKTYPE_Datagram);
//
//	IPAddressPtr ToServer = std::make_shared<IPAddress>();
//	ToServer->SetLoopbackAddress();
//	ToServer->SetPort(9000);
//
//	IPAddressPtr FromServer = std::make_shared<IPAddress>();
//	while (true)
//	{
//		int8 SendBuf[4096] = { "SEND TO CLIENT\n" };
//		ZeroMemory(SendBuf, 4096);
//		int32 SendSize = 0;
//
//		sock->SendTo(SendBuf, 4096, SendSize, ToServer);
//
//		std::wcout << "SEND TO CLIENT : " << ToServer->ToString() << std::endl;
//
//		int8 RecvBuf[4096];
//		ZeroMemory(RecvBuf, 4096);
//		int32 RecvSize = 0;
//
//		sock->RecvFrom(RecvBuf, 4096, RecvSize, FromServer);
//
//		if (RecvSize == 0)
//		{
//			printf("BytesRead is 0\n");
//			exit(0);
//		}
//
//		std::wcout << "RECV FROM CLIENT : " << FromServer->ToString() << " : " << RecvBuf << std::endl;
//
//		//Sleep(1000);
//	}
//
//	SocketUtils::DestroySocket(sock);
//}

int main(void)
{
	setlocale(LC_ALL, "");
	
	WinSocketPtr sock = StartUp();

	do
	{
		ENCODE_SINGING(sock);

		DECODE_SINGIN(sock);

		Sleep(33);

	} while (true);


	system("pause");

	SocketUtils::DestroySocket(sock);
	WSACleanup();
}