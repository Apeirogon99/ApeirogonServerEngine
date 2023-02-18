#include "pch.h"
#include "IdentityPlayerState.h"

RemotePlayer::RemotePlayer()
{
}

RemotePlayer::~RemotePlayer()
{
}

IdentityPlayerState::IdentityPlayerState()
{
}

IdentityPlayerState::~IdentityPlayerState()
{
}

void IdentityPlayerState::OnConnected()
{
	Protocol::S2C_EnterServer enterServer;
	PacketSessionPtr session = GetPacketSessionRef();

	bool valid = session->IsValid();
	if (false == valid)
	{
		enterServer.set_error(-1);
	}
	else
	{
		RemotePlayerPtr newRemotePlayer = std::make_shared<RemotePlayer>();
		PlayerStatePtr playerState = std::static_pointer_cast<IdentityPlayerState>(session);
		playerState->SetRemotePlayer(newRemotePlayer);
		enterServer.set_error(0);
	}

	SendBufferPtr sendBuffer = IdentityServerPacketHandler::MakeSendBuffer(session, enterServer);
	Send(sendBuffer);
}

void IdentityPlayerState::OnSend(uint32 len)
{

}

void IdentityPlayerState::OnDisconnected()
{
	Protocol::S2C_LeaveServer leavePacket;
	PacketSessionPtr session = GetPacketSessionRef();
	bool valid = session->IsValid();
	if (false == valid)
	{
		leavePacket.set_error(-1);
	}
	else
	{
		leavePacket.set_error(0);
	}

	SendBufferPtr sendBuffer = IdentityServerPacketHandler::MakeSendBuffer(session, leavePacket);
	Send(sendBuffer);
}

void IdentityPlayerState::OnRecvPacket(BYTE* buffer, const uint32 len)
{
	PacketSessionPtr session = GetPacketSessionRef();

	bool result = false;
	result = IdentityServerPacketHandler::HandlePacket(session, buffer, len);
	if (false == result)
	{
		this->SessionLog(L"IdentityPlayerState::OnRecvPacket() : Failed to handle packet\n");
		return;
	}
}

RemotePlayerPtr IdentityPlayerState::GetRemotePlayer()
{
	return mRemotePlayer;
}

void IdentityPlayerState::SetRemotePlayer(RemotePlayerPtr& inRemotePlayer)
{
	mRemotePlayer = std::move(inRemotePlayer);
}

int32 IdentityPlayerState::GetGlobalID()
{
	return mGlobalID;
}

void IdentityPlayerState::SetGlobalID(int32 inGlobalID)
{
	mGlobalID = inGlobalID;
}
