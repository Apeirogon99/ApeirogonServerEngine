#include "pch.h"
#include "IdentityClientPacketHandler.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionPtr& session, BYTE* buffer, int32 len)
{
	return false;
}

bool Handle_S2C_Singin(PacketSessionPtr& session, Protocol::S2C_Singin& pkt)
{
	return false;
}

bool Handle_S2C_Singup(PacketSessionPtr& session, Protocol::S2C_Singup& pkt)
{
	return false;
}

bool Handle_S2C_EmailVerified(PacketSessionPtr& session, Protocol::S2C_EmailVerified& pkt)
{
	return false;
}