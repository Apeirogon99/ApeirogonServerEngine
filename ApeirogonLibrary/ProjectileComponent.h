#pragma once
class APEIROGON_API ProjectileComponent
{
public:
	ProjectileComponent();
	~ProjectileComponent();

	ProjectileComponent(const ProjectileComponent&) = delete;
	ProjectileComponent(ProjectileComponent&&) noexcept = delete;

	ProjectileComponent& operator=(const ProjectileComponent&) = delete;
	ProjectileComponent& operator=(ProjectileComponent&&) noexcept = delete;

public:
	void InitProjectile(const Location& inInitLocation, const Rotation& inInitRotation, const int64& inMaxSyncTime, const int64& inWorldTime);
	bool Update(ActorPtr inOwner);
	bool SyncUpdate(ActorPtr inOwner, const int64 inSyncTime);

public:
	const Location	GetCurrentLocation(ActorPtr inOwner);
	const int64		GetLastMovementTime() const;

private:
	Location	mLocation;
	Rotation	mRotation;

	int64		mLastMovementTime;
	int64		mCurrentMovementSyncTime;
	int64		mMaxSyncTime;
};

