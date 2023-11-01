#pragma once
class Actor : public GameObject
{
public:
	APEIROGON_API Actor(const WCHAR* inActorName);
	APEIROGON_API virtual ~Actor();

public:
	APEIROGON_API virtual void	OnInitialization()				abstract;
	APEIROGON_API virtual void	OnDestroy()						abstract;
	APEIROGON_API virtual void	OnTick(const int64 inDeltaTime)	abstract;
	APEIROGON_API virtual bool	IsValid()						abstract;

public:
	APEIROGON_API void			CloseToActor(ActorPtr inCloseActor, float inPossibleVisbleLength);
	APEIROGON_API virtual void	OnAppearActor(ActorPtr inAppearActor) {};
	APEIROGON_API virtual void	OnDisAppearActor(ActorPtr inDisAppearActor) {};

	APEIROGON_API virtual void	OnInteractive(ActorPtr inActor) {};

	APEIROGON_API virtual void	OnHit(ActorPtr inInstigated, const float inDamage) {};
	APEIROGON_API virtual void	OnDeath() {};

	APEIROGON_API virtual void	OnAutoAttackShot(ActorPtr inVictim) {};
	APEIROGON_API virtual void	OnAutoAttackTargeting(const float inDamage, const FVector inRange) {};
	APEIROGON_API virtual void	OnAutoAttackOver() {};

public:
	APEIROGON_API void			SetWorld(WorldRef inWorld);

	APEIROGON_API void			SetTransform(const Transform& inTransform);
	APEIROGON_API void			SetTransform(const Location& inLocation, const Rotation& inRotation, const Scale& inScale);   

	APEIROGON_API void			SetLocation(const Location& inLocation);
	APEIROGON_API void			SetLocation(const float inX, const float inY, const float inZ);

	APEIROGON_API void			SetRotation(const Rotation& inRotator);
	APEIROGON_API void			SetRotation(const float inPitch, const float inYaw, const float inRoll);

	APEIROGON_API void			SetScale(const Scale& inScale);
	APEIROGON_API void			SetScale(const float inX, const float inY, const float inZ);

	APEIROGON_API void			SetVelocity(const FVector& inVelocity);
	APEIROGON_API void			SetVelocity(const float inX, const float inY, const float inZ);

	APEIROGON_API void			SetActorType(const uint8& inActorType);

	APEIROGON_API void			SetDefaultCollisionComponent(CollisionComponent* inCollisionComponent);

	APEIROGON_API bool			FindPlayerViewer(RemoteClientPtr inRemoteClient);
	APEIROGON_API bool			InsertPlayerViewer(RemoteClientPtr inRemoteClient);
	APEIROGON_API bool			ReleasePlayerViewer(RemoteClientPtr inRemoteClient);
	APEIROGON_API void			BrodcastPlayerViewers(SendBufferPtr inSendBuffer);
	APEIROGON_API void			ClearPlayerViewers();

public:
	APEIROGON_API ActorPtr				GetActorPtr()			{ return std::static_pointer_cast<Actor>(shared_from_this()); }
	APEIROGON_API ActorRef				GetActorRef()			{ return std::static_pointer_cast<Actor>(shared_from_this()); }
	APEIROGON_API WorldRef				GetWorld()		const	{ return mWorld; }

	APEIROGON_API const Transform&		GetTransform()	const	{ return mTransfrom; }
	APEIROGON_API const Location&		GetLocation()	const	{ return mTransfrom.GetLocation(); }
	APEIROGON_API const Rotation&		GetRotation()	const	{ return mTransfrom.GetRotation(); }
	APEIROGON_API const Scale&			GetScale()		const	{ return mTransfrom.GetScale(); }
	APEIROGON_API const Velocity&		GetVelocity()	const	{ return mVelocity; }
	APEIROGON_API const uint8&			GetActorType()	const	{ return mActorType; }
	APEIROGON_API const PlayerViewer&	GetPlayerViewers()		{ return mPlayerViewers; }
	APEIROGON_API CollisionComponent*	GetDefaultCollisionComponent() { return mDefaultCollisionComponent; }

protected:
	WorldRef		mWorld;
	Transform		mTransfrom;
	FVector			mVelocity;
	uint8			mActorType;
	PlayerViewer	mPlayerViewers;
	CollisionComponent* mDefaultCollisionComponent;
};

