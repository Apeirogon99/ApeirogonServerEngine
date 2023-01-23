#pragma once
//#include "Protocol.pb.h"

using PacketHandlerFunc = std::function<bool(PacketSessionPtr&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum class EPakcetID: uint16
{
	C2S_SINGIN = 1000,
	S2C_SINGIN = 1001,
	C2S_SINGUP = 1002,
	S2C_SINGUP = 1003,
	C2S_EMAILVERIFIED = 1004,
	S2C_EMAILVERIFIED = 1005,
};

// Custom Handlers
bool Handle_INVALID(PacketSessionPtr& session, BYTE* buffer, int32 len);
bool Handle_C2S_SINGIN(PacketSessionPtr& session, Protocol::C2S_SINGIN& pkt);
bool Handle_C2S_SINGUP(PacketSessionPtr& session, Protocol::C2S_SINGUP& pkt);
bool Handle_C2S_EMAILVERIFIED(PacketSessionPtr& session, Protocol::C2S_EMAILVERIFIED& pkt);

class TestPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_C2S_SINGIN] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_SINGIN>(Handle_C2S_SINGIN, session, buffer, len); };
		GPacketHandler[PKT_C2S_SINGUP] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_SINGUP>(Handle_C2S_SINGUP, session, buffer, len); };
		GPacketHandler[PKT_C2S_EMAILVERIFIED] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_EMAILVERIFIED>(Handle_C2S_EMAILVERIFIED, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionPtr& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferRef MakeSendBuffer(Protocol::S2C_SINGIN& pkt) { return MakeSendBuffer(pkt, PKT_S2C_SINGIN); }
	static SendBufferRef MakeSendBuffer(Protocol::S2C_SINGUP& pkt) { return MakeSendBuffer(pkt, PKT_S2C_SINGUP); }
	static SendBufferRef MakeSendBuffer(Protocol::S2C_EMAILVERIFIED& pkt) { return MakeSendBuffer(pkt, PKT_S2C_EMAILVERIFIED); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionPtr& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferPtr sendBuffer = GSendBufferManager->Open(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};