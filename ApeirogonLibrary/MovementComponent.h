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
	bool Update(ActorPtr inOwner, const float inCloseToDestination);
	bool SyncUpdate(const int64 inSyncTime);

public:
	void SetSynchronizationTime(const int64 inMovementSyncTime);
	void SetNewDestination(const Location& inDestinationLocation, const int64 inMovementLastTime);

public:
	const Location& GetDestinationLocation() const;
	const int64		GetLastMovementTime() const;

private:
	Location	mDestinationLocation;
	int64		mLastMovementTime;
	int64		mCurrentMovementSyncTime;
	int64		mMaxMovementSyncTime;
};

