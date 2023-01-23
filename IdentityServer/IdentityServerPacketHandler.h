#pragma once

using PacketHandlerFunc = std::function<bool(PacketSessionPtr&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum class EPakcetID: uint16
{
	C2S_Singin = 1000,
	S2C_Singin = 1001,
	C2S_Singup = 1002,
	S2C_Singup = 1003,
	C2S_EmailVerified = 1004,
	S2C_EmailVerified = 1005,
};

// Custom Handlers
bool Handle_INVALID(PacketSessionPtr& session, BYTE* buffer, int32 len);
bool Handle_C2S_Singin(PacketSessionPtr& session, Protocol::C2S_Singin& pkt);
bool Handle_C2S_Singup(PacketSessionPtr& session, Protocol::C2S_Singup& pkt);
bool Handle_C2S_EmailVerified(PacketSessionPtr& session, Protocol::C2S_EmailVerified& pkt);

class IdentityServerPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_Singin)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_Singin>(Handle_C2S_Singin, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_Singup)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_Singup>(Handle_C2S_Singup, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_EmailVerified)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_EmailVerified>(Handle_C2S_EmailVerified, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionPtr& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_Singin& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_Singin)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_Singup& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_Singup)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_EmailVerified& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_EmailVerified)); }

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
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SessionManagerPtr manager = session->GetSessionManager();
		SendRingBuffer& sendRingBuffer = manager->GetSendRingBuffer();
		SendBufferPtr sendBuffer = sendRingBuffer.Writer(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		pkt.SerializeToArray(&header[1], dataSize);
		sendRingBuffer.Reserve(packetSize);

		return sendBuffer;
	}
};