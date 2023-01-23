#include "pch.h"
#include "IdentityServerPacketHandler.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionPtr& session, BYTE* buffer, int32 len)
{
	return false;
}

bool Handle_C2S_Singin(PacketSessionPtr& session, Protocol::C2S_Singin& pkt)
{
	PlayerStatePtr playerState = std::static_pointer_cast<IdentityPlayerState>(session);
	if (nullptr == playerState)
	{
		return false;
	}

	ADOVariant clientID = pkt.id().c_str();
	ADOVariant clientPS = pkt.password().c_str();

	std::string idToken = "TEST ID TOKEN";

	Protocol::S2C_Singin sendPacket;
	sendPacket.set_error(0);
	sendPacket.set_id_token(idToken);

	SendBufferPtr sendBuffer = IdentityServerPacketHandler::MakeSendBuffer(session, sendPacket);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C2S_Singup(PacketSessionPtr& session, Protocol::C2S_Singup& pkt)
{
	return false;
}

bool Handle_C2S_EmailVerified(PacketSessionPtr& session, Protocol::C2S_EmailVerified& pkt)
{
	return false;
}
