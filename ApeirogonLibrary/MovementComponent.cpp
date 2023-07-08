#include "pch.h"
#include "MovementComponent.h"

MovementComponent::MovementComponent() : mDestinationLocation(), mLastMovementTime(0), mCurrentMovementSyncTime(0), mMaxMovementSyncTime(0)
{
}

MovementComponent::~MovementComponent()
{
}

bool MovementComponent::Update(ActorPtr inOwner, const float inCloseToDestination)
{

	Location currentLocation = inOwner->GetLocation();
	Location destinationLocation = this->mDestinationLocation;

	const float locationDistance = FVector::Distance2D(currentLocation, destinationLocation);
	if (locationDistance < inCloseToDestination)
	{
		inOwner->SetLocation(destinationLocation);
		return false;
	}

	Velocity	currentVelocity		= inOwner->GetVelocity();
	const int64 currentWorldTime	= inOwner->GetWorld().lock()->GetWorldTime();
	const float	duration			= static_cast<float>(currentWorldTime - this->mLastMovementTime) / 1000.0f;

	FVector	direction = destinationLocation - currentLocation;
	direction.Normalize();

	FRotator rotation = direction.Rotator();
	FVector velocity = direction * currentVelocity;

	FVector	deadReckoningLocation = currentLocation + (velocity * duration);

	//inOwner->GameObjectLog(L"[Move] (%5.6f:%5.6f:%5.6f)\n", deadReckoningLocation.GetX(), deadReckoningLocation.GetY(), deadReckoningLocation.GetZ());

	inOwner->SetLocation(deadReckoningLocation);
	inOwner->SetRotation(rotation);
	this->mLastMovementTime = currentWorldTime;
	return true;
}

bool MovementComponent::SyncUpdate(const int64 inSyncTime)
{
	mCurrentMovementSyncTime += inSyncTime;
	if (mCurrentMovementSyncTime > mMaxMovementSyncTime)
	{
		mCurrentMovementSyncTime = 0;
		return true;
	}

	return false;
}

void MovementComponent::SetSynchronizationTime(const int64 inMovementSyncTime)
{
	mMaxMovementSyncTime = inMovementSyncTime;
	mCurrentMovementSyncTime = 0;
}

void MovementComponent::SetNewDestination(const Location& inDestinationLocation, const int64 inMovementLastTime)
{
	mDestinationLocation = inDestinationLocation;
	mLastMovementTime = inMovementLastTime;
}

const Location& MovementComponent::GetDestinationLocation() const
{
	return mDestinationLocation;
}

const int64 MovementComponent::GetLastMovementTime() const
{
	return mLastMovementTime;
}
