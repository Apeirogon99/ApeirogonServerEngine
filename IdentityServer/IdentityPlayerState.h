#pragma once

class RemotePlayer
{
public:
	RemotePlayer();
	virtual ~RemotePlayer();

	RemotePlayer(const RemotePlayer& player) = delete;
	RemotePlayer& operator=(const RemotePlayer& player) = delete;

public:


private:
	std::set<CharacterPtr> mCharacters;
};

class IdentityPlayerState : public PacketSession
{
public:
	IdentityPlayerState();
	virtual ~IdentityPlayerState();

	IdentityPlayerState(const IdentityPlayerState& player) = delete;
	IdentityPlayerState& operator=(const IdentityPlayerState& player) = delete;

protected:
	virtual void OnConnected() override;
	virtual void OnSend(uint32 len) override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, const uint32 len) override;

public:
	RemotePlayerPtr GetRemotePlayer();
	void SetRemotePlayer(RemotePlayerPtr& inRemotePlayer);

	int32 GetGlobalID();
	void SetGlobalID(int32 inGlobalID);

private:
	RemotePlayerPtr	mRemotePlayer;
	int32			mGlobalID = -1;
};

