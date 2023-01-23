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
	uint32 processLen = 0;

	while (true)
	{
		int32 dataSize = len - processLen;

		if (dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header;
		buffer.Peek(reinterpret_cast<BYTE*>(&header), sizeof(PacketHeader));
		if (dataSize < header.size)
			break;

		OnRecvPacket(buffer, header.size);

		buffer.MoveFront(header.size);

		processLen += header.size;
	}

	return processLen;
}
