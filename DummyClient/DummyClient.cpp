#include "pch.h"
#include "IdentityPacket.pb.h"
#include "ClientPacketHandler.h"

struct PacketHeader
{
	uint16 packetType;
	uint16 packetSize;
};

static int32 packetHeaderSize = static_cast<int32>(sizeof(PacketHeader));

Socket* StartUp()
{
	std::wstring ipAddress = L"127.0.0.1";
	uint16 port = 9000;

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	WSAStartup(ver, &wsData);

	Socket* sock = SocketUtils::CreateSocket(EProtocolType::IPv4, ESocketType::SOCKTYPE_Streaming);

	IPAddress sockAddr;
	sockAddr.SetIp(ipAddress, 9000);

	sock->Connect(sockAddr);

	return sock;
}

void ENCODE_SINGING(Socket * sock)
{
	Identity::C2S_Singin C2S_pktSingin;
	C2S_pktSingin.set_id("playerID");
	C2S_pktSingin.set_password("PlayerPassword");

	const uint16 dataSize = static_cast<uint16>(C2S_pktSingin.ByteSizeLong());
	const uint16 packetSize = static_cast<uint16>(dataSize + packetHeaderSize);

	PacketHeader header;
	header.packetType = C2S_SINGIN;
	header.packetSize = packetSize;

	char* sendBuf = new char[packetSize];
	ZeroMemory(sendBuf, packetSize);

	memcpy(sendBuf, &header, packetHeaderSize);
	C2S_pktSingin.SerializeToArray(sendBuf + packetHeaderSize, dataSize);

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

	Identity::S2C_Singin S2C_SinginRECV;
	S2C_SinginRECV.ParseFromArray(Recvbuf + packetHeaderSize, headerRecv.packetSize - packetHeaderSize);
	std::cout << "SERVER > " << S2C_SinginRECV.error() << " : " << S2C_SinginRECV.idtoken() << std::endl;

}

int main(void)
{
	setlocale(LC_ALL, "");

	Socket* sock = StartUp();

	do
	{
		ENCODE_SINGING(sock);

		DECODE_SINGIN(sock);

		Sleep(1000);

	} while (true);

	system("pause");

	SocketUtils::DestroySocket(sock);
	WSACleanup();
}