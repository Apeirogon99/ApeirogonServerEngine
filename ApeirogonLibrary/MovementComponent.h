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
	void SetNewDestination(const Location& inDestinationLocation);
	bool Update(ActorPtr& inOwner, const float inCloseToDestination);

public:

private:
	Location	mDestinationLocation;
	int64		mLastMovementLocation;
};

