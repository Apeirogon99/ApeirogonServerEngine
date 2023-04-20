#pragma once

struct PacketHeader
{
	uint16 size;
	uint16 id;
};

class PacketSession : public Session
{
public:
	APEIROGON_API PacketSession();
	APEIROGON_API virtual ~PacketSession();

	APEIROGON_API PacketSessionPtr	GetPacketSessionRef();

protected:
	APEIROGON_API virtual uint32	OnRecv(RingBuffer& inRingBuffer, uint32 inLen) sealed;
	APEIROGON_API virtual void		OnRecvPacket(BYTE* buffer, const uint32 len) abstract;
};
