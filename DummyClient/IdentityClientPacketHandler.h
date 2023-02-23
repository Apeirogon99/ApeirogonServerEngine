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
bool Handle_S2C_EnterServer(PacketSessionPtr& session, Protocol::S2C_EnterServer& pkt);
bool Handle_S2C_LeaveServer(PacketSessionPtr& session, Protocol::S2C_LeaveServer& pkt);
bool Handle_S2C_Singin(PacketSessionPtr& session, Protocol::S2C_Singin& pkt);
bool Handle_S2C_Singup(PacketSessionPtr& session, Protocol::S2C_Singup& pkt);
bool Handle_S2C_EmailVerified(PacketSessionPtr& session, Protocol::S2C_EmailVerified& pkt);
bool Handle_S2C_LoadServer(PacketSessionPtr& session, Protocol::S2C_LoadServer& pkt);
bool Handle_S2C_LoadCharacters(PacketSessionPtr& session, Protocol::S2C_LoadCharacters& pkt);
bool Handle_S2C_SelectServer(PacketSessionPtr& session, Protocol::S2C_SelectServer& pkt);
bool Handle_S2C_CreateCharacter(PacketSessionPtr& session, Protocol::S2C_CreateCharacter& pkt);
bool Handle_S2C_AppearanceCharacter(PacketSessionPtr& session, Protocol::S2C_AppearanceCharacter& pkt);
bool Handle_S2C_DeleteCharacter(PacketSessionPtr& session, Protocol::S2C_DeleteCharacter& pkt);
bool Handle_S2C_UpdateNickName(PacketSessionPtr& session, Protocol::S2C_UpdateNickName& pkt);
bool Handle_S2C_TravelServer(PacketSessionPtr& session, Protocol::S2C_TravelServer& pkt);

class IdentityClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[static_cast<uint16>(EPakcetID::S2C_EnterServer)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_EnterServer>(Handle_S2C_EnterServer, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::S2C_LeaveServer)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_LeaveServer>(Handle_S2C_LeaveServer, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::S2C_Singin)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_Singin>(Handle_S2C_Singin, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::S2C_Singup)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_Singup>(Handle_S2C_Singup, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::S2C_EmailVerified)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_EmailVerified>(Handle_S2C_EmailVerified, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::S2C_LoadServer)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_LoadServer>(Handle_S2C_LoadServer, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::S2C_LoadCharacters)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_LoadCharacters>(Handle_S2C_LoadCharacters, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::S2C_SelectServer)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_SelectServer>(Handle_S2C_SelectServer, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::S2C_CreateCharacter)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_CreateCharacter>(Handle_S2C_CreateCharacter, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::S2C_AppearanceCharacter)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_AppearanceCharacter>(Handle_S2C_AppearanceCharacter, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::S2C_DeleteCharacter)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_DeleteCharacter>(Handle_S2C_DeleteCharacter, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::S2C_UpdateNickName)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_UpdateNickName>(Handle_S2C_UpdateNickName, session, buffer, len); };
		GPacketHandler[static_cast<uint16>(EPakcetID::S2C_TravelServer)] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S2C_TravelServer>(Handle_S2C_TravelServer, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionPtr& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::C2S_EnterServer& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::C2S_EnterServer)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::C2S_LeaveServer& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::C2S_LeaveServer)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::C2S_Singin& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::C2S_Singin)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::C2S_Singup& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::C2S_Singup)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::C2S_EmailVerified& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::C2S_EmailVerified)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::C2S_LoadServer& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::C2S_LoadServer)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::C2S_LoadCharacters& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::C2S_LoadCharacters)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::C2S_SelectServer& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::C2S_SelectServer)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::C2S_CreateCharacter& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::C2S_CreateCharacter)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::C2S_AppearanceCharacter& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::C2S_AppearanceCharacter)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::C2S_DeleteCharacter& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::C2S_DeleteCharacter)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::C2S_UpdateNickName& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::C2S_UpdateNickName)); }
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::C2S_TravelServer& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::C2S_TravelServer)); }

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