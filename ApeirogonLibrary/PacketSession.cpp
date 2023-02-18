#include "pch.h"
#include "PacketSession.h"

PacketSession::PacketSession()
{
}

PacketSession::~PacketSession()
{
}

PacketSessionPtr PacketSession::GetPacketSessionRef()
{
	return std::static_pointer_cast<PacketSession>(shared_from_this());
}

uint32 PacketSession::OnRecv(RecvRingBuffer& buffer, uint32 len)
{
	BYTE packetBuffer[1024];
	uint32 processLen = 0;

	while (true)
	{
		int32 dataSize = len - processLen;

		if (dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header;
		buffer.Peek(reinterpret_cast<BYTE*>(&header), sizeof(PacketHeader));
		const uint16 packetSize = header.size;
		if (dataSize < packetSize)
			break;

		memset(packetBuffer, NULL, 1024);
		buffer.Dequeue(packetBuffer, packetSize);

		OnRecvPacket(packetBuffer, packetSize);

		//buffer.MoveFront(packetSize);

		processLen += packetSize;
	}

	return processLen;
}
