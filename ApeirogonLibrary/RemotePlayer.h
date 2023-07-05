#pragma once
class RemotePlayer : public GameObject
{
public:
	APEIROGON_API RemotePlayer();
	APEIROGON_API virtual ~RemotePlayer();

	RemotePlayer(const RemotePlayer&) = delete;
	RemotePlayer(RemotePlayer&&) = delete;

	RemotePlayer& operator=(const RemotePlayer&) = delete;
	RemotePlayer& operator=(RemotePlayer&&) = delete;

public:
	APEIROGON_API virtual void	OnInitialization()				abstract;
	APEIROGON_API virtual void	OnDestroy()						abstract;
	APEIROGON_API virtual void	OnTick(const int64 inDeltaTime)	abstract;
	APEIROGON_API virtual bool	IsValid()						abstract;

public:
	APEIROGON_API void			SetRemoteClient(RemoteClientRef inRemoteClient);

	APEIROGON_API bool			InsertPlayerViewer(RemoteClientPtr inRemoteClient);
	APEIROGON_API bool			ReleasePlayerViewer(RemoteClientPtr inRemoteClient);
	APEIROGON_API void			BrodcastPlayerViewers(SendBufferPtr inSendBuffer);

public:
	APEIROGON_API RemotePlayerRef	GetRemotePlayer()			{ return std::static_pointer_cast<RemotePlayer>(shared_from_this()); }
	APEIROGON_API RemoteClientRef	GetRemoteClient()	const	{ return mRemoteClient; }
	APEIROGON_API PlayerViewer&		GetViewers()				{ return mPlayerViewers; }

private:
	RemoteClientRef	mRemoteClient;
	PlayerViewer	mPlayerViewers;
};

