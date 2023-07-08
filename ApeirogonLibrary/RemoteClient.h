#pragma once
class RemoteClient : public PacketSession
{
public:
	APEIROGON_API RemoteClient();
	APEIROGON_API virtual ~RemoteClient();

	RemoteClient(const RemoteClient&) = delete;
	RemoteClient& operator=(const RemoteClient&) = delete;

	RemoteClient(RemoteClient&&) = delete;
	RemoteClient& operator=(RemoteClient&&) = delete;

protected:
	APEIROGON_API virtual void		OnConnected()									abstract;
	APEIROGON_API virtual void		OnSend(uint32 len)								abstract;
	APEIROGON_API virtual void		OnIcmp()										abstract;
	APEIROGON_API virtual void		OnDisconnected()								abstract;
	APEIROGON_API virtual void		OnRecvPacket(BYTE* buffer, const uint32 len)	abstract;

public:
	APEIROGON_API void				SetRemotePlayer(RemotePlayerPtr inRemotePlayer);

	APEIROGON_API bool				FindPlayerMonitor(RemotePlayerPtr inRemotePlayer);
	APEIROGON_API bool				InsertPlayerMonitor(RemotePlayerPtr inRemotePlayer);
	APEIROGON_API bool				ReleasePlayerMonitor(RemotePlayerPtr inRemotePlayer);
	APEIROGON_API void				BroadcastPlayerMonitors(SendBufferPtr inSendBuffer);

	APEIROGON_API bool				FindActorMonitor(ActorPtr inActor);
	APEIROGON_API bool				InsertActorMonitor(ActorPtr inActor);
	APEIROGON_API bool				ReleaseActorMonitor(ActorPtr inActor);

public:
	APEIROGON_API RemoteClientRef	GetRemoteClientRef() { return std::static_pointer_cast<RemoteClient>(shared_from_this()); }

	APEIROGON_API RemotePlayerPtr&		GetRemotePlayer()	 { return mRemotePlayer; }
	APEIROGON_API const PlayerMonitors&	GetPlayerMonitors()	 { return mPlayerMonitors; }
	APEIROGON_API const ActorMonitors&	GetActorMonitors()	 { return mActorMonitors; }

private:
	RemotePlayerPtr		mRemotePlayer;
	PlayerMonitors		mPlayerMonitors;
	ActorMonitors		mActorMonitors;
};