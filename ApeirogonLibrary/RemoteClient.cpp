#include "pch.h"
#include "RemoteClient.h"

RemoteClient::RemoteClient()
{
}

RemoteClient::~RemoteClient()
{
}

void RemoteClient::SetRemotePlayer(RemotePlayerPtr inRemotePlayer)
{
	mRemotePlayer = inRemotePlayer;
}

void RemoteClient::BroadcastPlayerMonitors(SendBufferPtr inSendBuffer)
{
	for (auto remotePlayer : mPlayerMonitors)
	{
		RemoteClientPtr remoteClient = remotePlayer->GetPlayerState().lock();
		if (nullptr != remoteClient)
		{
			remoteClient->Send(inSendBuffer);
		}
	}
}