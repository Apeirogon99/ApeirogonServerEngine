#pragma once

using PacketHandlerFunc = std::function<bool(PacketSessionPtr&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum class EPakcetID: uint16
{
	C2S_EnterServer = 1000,
	S2C_EnterServer = 1001,
	C2S_LeaveServer = 1002,
	S2C_LeaveServer = 1003,
	C2S_Singin = 1004,
	S2C_Singin = 1005,
	C2S_Singup = 1006,
	S2C_Singup = 1007,
	C2S_EmailVerified = 1008,
	S2C_EmailVerified = 1009,
	C2S_LoadServer = 1010,
	S2C_LoadServer = 1011,
	C2S_LoadCharacters = 1012,
	S2C_LoadCharacters = 1013,
	C2S_SelectServer = 1014,
	S2C_SelectServer = 1015,
	C2S_CreateCharacter = 1016,
	S2C_CreateCharacter = 1017,
	C2S_AppearanceCharacter = 1018,
	S2C_AppearanceCharacter = 1019,
	C2S_DeleteCharacter = 1020,
	S2C_DeleteCharacter = 1021,
	C2S_UpdateNickName = 1022,
	S2C_UpdateNickName = 1023,
	C2S_TravelServer = 1024,
	S2C_TravelServer = 1025,
};

// Custom Handlers
bool Handle_INVALID(PacketSessionPtr& session, BYTE* buffer, int32 len);
bool Handle_C2S_EnterServer(PacketSessionPtr& session, Protocol::C2S_EnterServer& pkt);
bool Handle_C2S_LeaveServer(PacketSessionPtr& session, Protocol::C2S_LeaveServer& pkt);
bool Handle_C2S_Singin(PacketSessionPtr& session, Protocol::C2S_Singin& pkt);
bool Handle_C2S_Singup(PacketSessionPtr& session, Protocol::C2S_Singup& pkt);
bool Handle_C2S_EmailVerified(PacketSessionPtr& session, Protocol::C2S_EmailVerified& pkt);
bool Handle_C2S_LoadServer(PacketSessionPtr& session, Protocol::C2S_LoadServer& pkt);
bool Handle_C2S_LoadCharacters(PacketSessionPtr& session, Protocol::C2S_LoadCharacters& pkt);
bool Handle_C2S_SelectServer(PacketSessionPtr& session, Protocol::C2S_SelectServer& pkt);
bool Handle_C2S_CreateCharacter(PacketSessionPtr& session, Protocol::C2S_CreateCharacter& pkt);
bool Handle_C2S_AppearanceCharacter(PacketSessionPtr& session, Protocol::C2S_AppearanceCharacter& pkt);
bool Handle_C2S_DeleteCharacter(PacketSessionPtr& session, Protocol::C2S_DeleteCharacter& pkt);
bool Handle_C2S_UpdateNickName(PacketSessionPtr& session, Protocol::C2S_UpdateNickName& pkt);
bool Handle_C2S_TravelServer(PacketSessionPtr& session, Protocol::C2S_TravelServer& pkt);

class IdentityServerPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_EnterServer)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_EnterServer>(Handle_C2S_EnterServer, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_LeaveServer)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_LeaveServer>(Handle_C2S_LeaveServer, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_Singin)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_Singin>(Handle_C2S_Singin, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_Singup)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_Singup>(Handle_C2S_Singup, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_EmailVerified)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_EmailVerified>(Handle_C2S_EmailVerified, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_LoadServer)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_LoadServer>(Handle_C2S_LoadServer, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_LoadCharacters)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_LoadCharacters>(Handle_C2S_LoadCharacters, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_SelectServer)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_SelectServer>(Handle_C2S_SelectServer, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_CreateCharacter)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_CreateCharacter>(Handle_C2S_CreateCharacter, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_AppearanceCharacter)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_AppearanceCharacter>(Handle_C2S_AppearanceCharacter, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_DeleteCharacter)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_DeleteCharacter>(Handle_C2S_DeleteCharacter, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_UpdateNickName)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_UpdateNickName>(Handle_C2S_UpdateNickName, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::C2S_TravelServer)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C2S_TravelServer>(Handle_C2S_TravelServer, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionPtr& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_EnterServer& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_EnterServer)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_LeaveServer& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_LeaveServer)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_Singin& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_Singin)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_Singup& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_Singup)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_EmailVerified& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_EmailVerified)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_LoadServer& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_LoadServer)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_LoadCharacters& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_LoadCharacters)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_SelectServer& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_SelectServer)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_CreateCharacter& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_CreateCharacter)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_AppearanceCharacter& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_AppearanceCharacter)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_DeleteCharacter& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_DeleteCharacter)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_UpdateNickName& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_UpdateNickName)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::S2C_TravelServer& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::S2C_TravelServer)); }

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