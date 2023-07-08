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

bool RemoteClient::FindPlayerMonitor(RemotePlayerPtr inRemotePlayer)
{
	auto result = mPlayerMonitors.find(inRemotePlayer);
	return result != mPlayerMonitors.end();
}

bool RemoteClient::InsertPlayerMonitor(RemotePlayerPtr inRemotePlayer)
{
	auto result = mPlayerMonitors.insert(inRemotePlayer);
	return result.second;
}

bool RemoteClient::ReleasePlayerMonitor(RemotePlayerPtr inRemotePlayer)
{
	size_t result = mPlayerMonitors.erase(inRemotePlayer);
	return result != 0;
}

void RemoteClient::BroadcastPlayerMonitors(SendBufferPtr inSendBuffer)
{
	for (auto remotePlayer : mPlayerMonitors)
	{
		RemoteClientPtr remoteClient = remotePlayer->GetRemoteClient().lock();
		if (nullptr != remoteClient)
		{
			remoteClient->Send(inSendBuffer);
		}
	}
}

bool RemoteClient::FindActorMonitor(ActorPtr inActor)
{
	auto result = mActorMonitors.find(inActor);
	return result != mActorMonitors.end();
}

bool RemoteClient::InsertActorMonitor(ActorPtr inActor)
{
	auto result = mActorMonitors.insert(inActor);
	return result.second;
}

bool RemoteClient::ReleaseActorMonitor(ActorPtr inActor)
{
	size_t result = mActorMonitors.erase(inActor);
	return result != 0;
}
