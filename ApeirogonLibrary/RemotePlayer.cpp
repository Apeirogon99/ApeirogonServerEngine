#include "pch.h"
#include "RemotePlayer.h"

RemotePlayer::RemotePlayer() : GameObject(L"RemotePlayer")
{

}
RemotePlayer::~RemotePlayer()
{

}

void RemotePlayer::SetRemoteClient(RemoteClientRef inRemoteClient)
{
	mRemoteClient = inRemoteClient;
}

bool RemotePlayer::FindPlayerViewer(RemoteClientPtr inRemoteClient)
{
	auto result = mPlayerViewers.find(inRemoteClient);
	return result != mPlayerViewers.end();
}

bool RemotePlayer::InsertPlayerViewer(RemoteClientPtr inRemoteClient)
{
	auto result = mPlayerViewers.insert(inRemoteClient);
	return result.second;
}

bool RemotePlayer::ReleasePlayerViewer(RemoteClientPtr inRemoteClient)
{
	size_t result = mPlayerViewers.erase(inRemoteClient);
	return result != 0;
}

void RemotePlayer::BrodcastPlayerViewers(SendBufferPtr inSendBuffer)
{
	for (auto viewer : mPlayerViewers)
	{
		viewer->Send(inSendBuffer);
	}
}
