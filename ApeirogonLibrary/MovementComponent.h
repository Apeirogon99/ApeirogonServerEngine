#pragma once
class APEIROGON_API MovementComponent
{
public:
	MovementComponent();
	~MovementComponent();

	MovementComponent(const MovementComponent&) = delete;
	MovementComponent(MovementComponent&&) noexcept = delete;

	MovementComponent& operator=(const MovementComponent&) = delete;
	MovementComponent& operator=(MovementComponent&&) noexcept = delete;

public:
	void InitMovement(const Location& inInitLocation, const int64& inMovementMaxSyncTime);
	bool Update(ActorPtr inOwner, const float& inCloseToDestination);
	bool SyncUpdate(ActorPtr inOwner, const int64 inSyncTime);

public:
	void SetNewDestination(ActorPtr inOwner, const Location& inCurrentLocation, Location& inDestinationLocation, const int64 inMovementLastTime, const float& inCollisionSize);

public:
	const Location& GetServerDestinationLocation() const;
	const Location& GetDestinationLocation() const;
	const int64		GetLastMovementTime() const;

private:
	Location	mDestinationLocation;
	Location	mServerDestinationLocation;
	Location	mOldLocation;
	int64		mLastMovementTime;
	int64		mCurrentMovementSyncTime;
	int64		mMaxMovementSyncTime;
};

