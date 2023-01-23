#pragma once

using PacketHandlerFunc = std::function<bool(PacketSessionPtr&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum class EPakcetID: uint16
{
{%- for pkt in parser.total_pkt %}
	{{pkt.name}} = {{pkt.id}},
{%- endfor %}
};

// Custom Handlers
bool Handle_INVALID(PacketSessionPtr& session, BYTE* buffer, int32 len);

{%- for pkt in parser.recv_pkt %}
bool Handle_{{pkt.name}}(PacketSessionPtr& session, Protocol::{{pkt.name}}& pkt);
{%- endfor %}

class {{output}}
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;

{%- for pkt in parser.recv_pkt %}
		GPacketHandler[static_cast<uint16>(EPakcetID::{{pkt.name}})] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::{{pkt.name}}>(Handle_{{pkt.name}}, session, buffer, len); };
{%- endfor %}
	}

	static bool HandlePacket(PacketSessionPtr& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}

{%- for pkt in parser.send_pkt %}
	static SendBufferPtr MakeSendBuffer(PacketSessionPtr& session, Protocol::{{pkt.name}}& pkt) { return MakeSendBuffer(session, pkt, static_cast<uint16>(EPakcetID::{{pkt.name}})); }
{%- endfor %}

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
