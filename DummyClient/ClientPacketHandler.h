#pragma once
//#include "Protocol.pb.h"

//using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
//extern PacketHandlerFunc GPacketHandler[UINT16_MAX];
//
enum : uint16
{
	C2S_SINGIN = 1000,
	S2C_SINGIN = 1001,
	C2S_SINGUP = 1002,
	S2C_SINGUP = 1003,
	C2S_EMAILVERIFIED = 1004,
	S2C_EMAILVERIFIED = 1005,
};
//
//// Custom Handlers
//bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
//bool Handle_S2C_SINGIN(PacketSessionRef& session, Protocol::S2C_SINGIN& pkt);
//bool Handle_S2C_SINGUP(PacketSessionRef& session, Protocol::S2C_SINGUP& pkt);
//bool Handle_S2C_EMAILVERIFIED(PacketSessionRef& session, Protocol::S2C_EMAILVERIFIED& pkt);
//
//class ClientPacketHandler
//{
//public:
//	static void Init()
//	{
//		for (int32 i = 0; i < UINT16_MAX; i++)
//			GPacketHandler[i] = Handle_INVALID;
//		GPacketHandler[PKT_S2C_SINGIN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_SINGIN>(Handle_S2C_SINGIN, session, buffer, len); };
//		GPacketHandler[PKT_S2C_SINGUP] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_SINGUP>(Handle_S2C_SINGUP, session, buffer, len); };
//		GPacketHandler[PKT_S2C_EMAILVERIFIED] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_EMAILVERIFIED>(Handle_S2C_EMAILVERIFIED, session, buffer, len); };
//	}
//
//	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
//	{
//		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
//		return GPacketHandler[header->id](session, buffer, len);
//	}
//	static SendBufferRef MakeSendBuffer(Protocol::C2S_SINGIN& pkt) { return MakeSendBuffer(pkt, PKT_C2S_SINGIN); }
//	static SendBufferRef MakeSendBuffer(Protocol::C2S_SINGUP& pkt) { return MakeSendBuffer(pkt, PKT_C2S_SINGUP); }
//	static SendBufferRef MakeSendBuffer(Protocol::C2S_EMAILVERIFIED& pkt) { return MakeSendBuffer(pkt, PKT_C2S_EMAILVERIFIED); }
//
//private:
//	template<typename PacketType, typename ProcessFunc>
//	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
//	{
//		PacketType pkt;
//		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
//			return false;
//
//		return func(session, pkt);
//	}
//
//	template<typename T>
//	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
//	{
//		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
//		const uint16 packetSize = dataSize + sizeof(PacketHeader);
//
//		SendBufferRef sendBuffer = GSendBufferManager->Open(packetSize);
//		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
//		header->size = packetSize;
//		header->id = pktId;
//		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
//		sendBuffer->Close(packetSize);
//
//		return sendBuffer;
//	}
//};