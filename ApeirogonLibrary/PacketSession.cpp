#include "pch.h"
#include "PacketSession.h"

PacketSession::PacketSession()
{
	mTempPacketBuffer = new BYTE[1024]();
}

PacketSession::~PacketSession()
{
}

PacketSessionPtr PacketSession::GetPacketSessionRef()
{
	return std::static_pointer_cast<PacketSession>(shared_from_this());
}

uint32 PacketSession::OnRecv(RingBuffer& inRingBuffer, uint32 inLen)
{
	uint32 processLen = 0;
	while (true)
	{
		int32 dataSize = inLen - processLen;

		if (dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header;
		inRingBuffer.Peek(reinterpret_cast<BYTE*>(&header), sizeof(PacketHeader));
		const uint16 packetSize = header.size;
		if (dataSize < packetSize)
			break;

		memset(mTempPacketBuffer, 0, 1024);
		inRingBuffer.Dequeue(mTempPacketBuffer, packetSize);

		OnRecvPacket(mTempPacketBuffer, packetSize);

		processLen += packetSize;
	}

	return processLen;
}