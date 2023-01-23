#pragma once
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
	virtual void OnRecvPacket(RecvRingBuffer& buffer, const uint32 len) override;


private:
	CharacterPtr mCharacter;
};

