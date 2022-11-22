#include "pch.h"
#include "IdentityPacket.pb.h"
#include "IdentityPacketHandler.h"

struct PacketHeader
{
	uint16 packetType;
	uint16 packetSize;
};

static int32 packetHeaderSize = static_cast<int32>(sizeof(PacketHeader));

Socket*  StartUp()
{
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

	return clientSocket;
}

void ENCODE_SINGING(Socket* sock)
{
	Identity::S2C_Singin S2C_pktSingin;
	S2C_pktSingin.set_error(0);
	S2C_pktSingin.set_idtoken(std::string("jierieriewirjiwjvjpirwjvipwri"));

	const uint16 dataSize = static_cast<uint16>(S2C_pktSingin.ByteSizeLong());
	const uint16 packetSize = dataSize + packetHeaderSize;

	PacketHeader header;
	header.packetType = C2S_SINGIN;
	header.packetSize = packetSize;

	char* sendBuf = new char[packetSize];
	ZeroMemory(sendBuf, packetSize);

	memcpy(sendBuf, &header, packetHeaderSize);
	S2C_pktSingin.SerializeToArray(sendBuf + packetHeaderSize, dataSize);

	int32 BytesSent = 0;
	sock->Send(sendBuf, (dataSize + packetHeaderSize), BytesSent);
}

void DECODE_SINGIN(Socket* sock)
{
	int8 Recvbuf[4096];
	int32 BytesRead = 0;

	ZeroMemory(Recvbuf, 4096);
	sock->Recv(Recvbuf, 4096, BytesRead);

	if (BytesRead == 0)
	{
		exit(0);
	}

	PacketHeader headerRecv;
	memcpy(&headerRecv, Recvbuf, packetHeaderSize);

	Identity::C2S_Singin C2S_pktSingin;
	C2S_pktSingin.ParseFromArray(Recvbuf + packetHeaderSize, headerRecv.packetSize - packetHeaderSize);
	std::cout << "CLIENT > " << C2S_pktSingin.id() << " : " << C2S_pktSingin.password() << std::endl;

}

int main(void)
{
	setlocale(LC_ALL, "");

	Socket* clientSocket = StartUp();

	while (clientSocket)
	{
		DECODE_SINGIN(clientSocket);

		ENCODE_SINGING(clientSocket);
	}

	system("pause");

	SocketUtils::DestroySocket(clientSocket);
	WSACleanup();
}