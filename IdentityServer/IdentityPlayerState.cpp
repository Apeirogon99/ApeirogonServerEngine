#include "pch.h"
#include "IdentityPlayerState.h"

IdentityPlayerState::IdentityPlayerState()
{
}

IdentityPlayerState::~IdentityPlayerState()
{
}

void IdentityPlayerState::OnConnected()
{
}

void IdentityPlayerState::OnSend(uint32 len)
{
}

void IdentityPlayerState::OnDisconnected()
{
}

void IdentityPlayerState::OnRecvPacket(RecvRingBuffer& buffer, const uint32 len)
{
	PacketSessionPtr session = GetPacketSessionRef();
	//PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer.GetReadBuffer());

	// TODO : packetId 대역 체크
	bool result = false;
	result = IdentityServerPacketHandler::HandlePacket(session, buffer.GetReadBuffer(), len);
	if (false == result)
	{
		this->SessionLog(L"IdentityPlayerState::OnRecvPacket() : Failed to handle packet");
		return;
	}

	//PacketHeader headerRecv;
	//buffer.Dequeue(reinterpret_cast<BYTE*>(&headerRecv), sizeof(PacketHeader));

	//const uint32 dataSize = headerRecv.size - sizeof(PacketHeader);

	//Identity::C2S_Singin C2S_pktSingin;
	//C2S_pktSingin.ParseFromArray(buffer.GetReadBuffer(), dataSize);
	//buffer.MoveFront(dataSize);

	//std::string id = C2S_pktSingin.id();
	//std::string ps = C2S_pktSingin.password();

	//printf("Server Recv : [%d (Byte)] [ID : %s][PS : %s]\n", len, id.c_str(), ps.c_str());

	////SEND
	//SessionManagerPtr sessionManger = GetSessionManager();
	//if (nullptr == sessionManger)
	//{
	//	return;
	//}

	//Identity::S2C_Singin S2C_pktSingin;
	//S2C_pktSingin.set_idtoken("TEST TOKEN");

	//const uint16 S2CdataSize = static_cast<uint16>(S2C_pktSingin.ByteSizeLong());
	//const uint16 S2CpacketSize = S2CdataSize + sizeof(PacketHeader);

	//SendRingBuffer& sendRingBuffer = sessionManger->GetSendRingBuffer();
	//SendBufferPtr S2CBuffer = std::make_shared<SendBuffer>(sendRingBuffer.Writer(S2CpacketSize), S2CpacketSize);

	//PacketHeader* header = reinterpret_cast<PacketHeader*>(S2CBuffer->Buffer());
	//header->size = S2CpacketSize;
	//header->id = S2C_SINGIN;


	//S2C_pktSingin.SerializeToArray(&header[1], S2CdataSize);

	//sendRingBuffer.Reserve(S2CpacketSize);


	//for (int i = 0; i < (rand() % 10) + 1; ++i)
	//{
	//	Send(S2CBuffer);
	//}
}
