#include "pch.h"
#include "MovementComponent.h"

MovementComponent::MovementComponent() : mDestinationLocation(), mOldLocation(), mServerDestinationLocation(), mLastMovementTime(0), mCurrentMovementSyncTime(0), mMaxMovementSyncTime(0)
{
}

MovementComponent::~MovementComponent()
{
}

void MovementComponent::InitMovement(const Location& inInitLocation, const int64& inMovementMaxSyncTime)
{
	mDestinationLocation		= inInitLocation;
	mServerDestinationLocation	= inInitLocation;
	mOldLocation				= inInitLocation;
	mLastMovementTime			= 0;
	mMaxMovementSyncTime		= inMovementMaxSyncTime;
	mCurrentMovementSyncTime	= 0;
}

bool MovementComponent::Update(ActorPtr inOwner, const float& inCloseToDestination)
{

	Location currentLocation = inOwner->GetLocation();
	Location destinationLocation = this->mServerDestinationLocation;

	if (currentLocation == destinationLocation)
	{
		return false;
	}

	const float locationDistance = FVector::Distance2D(currentLocation, destinationLocation);
	if (locationDistance <= inCloseToDestination)
	{
		inOwner->SetLocation(destinationLocation);
		return false;
	}

	Velocity	currentVelocity		= inOwner->GetVelocity();
	const int64 currentWorldTime	= inOwner->GetWorld().lock()->GetWorldTime();
	const float	duration			= static_cast<float>(currentWorldTime - this->mLastMovementTime) / 1000.0f;

	FVector	direction = destinationLocation - currentLocation;
	direction.Normalize();
	FVector velocity = direction * currentVelocity;

	FVector	deadReckoningLocation = currentLocation + (velocity * duration);

	//inOwner->GameObjectLog(L"[Move] (%5.6f:%5.6f:%5.6f)\n", deadReckoningLocation.GetX(), deadReckoningLocation.GetY(), deadReckoningLocation.GetZ());

	this->mOldLocation = currentLocation;

	inOwner->SetLocation(deadReckoningLocation);
	this->mLastMovementTime = currentWorldTime;
	return true;
}

bool MovementComponent::SyncUpdate(ActorPtr inOwner, const int64 inSyncTime)
{
	mCurrentMovementSyncTime += inSyncTime;
	if (mCurrentMovementSyncTime <= mMaxMovementSyncTime)
	{
		return false;
	}
	mCurrentMovementSyncTime = 0;

	const float distance = FVector::Distance2D(inOwner->GetLocation(), this->mOldLocation);
	if (distance <= 0.1f)
	{
		return false;
	}

	mOldLocation = inOwner->GetLocation();
	return true;
}

void MovementComponent::SetNewDestination(ActorPtr inOwner, const Location& inCurrentLocation, Location& inDestinationLocation, const int64 inMovementLastTime, const float& inCollisionSize)
{
	FVector		direction = inDestinationLocation - inCurrentLocation;
	FRotator	rotation = direction.Rotator();
	FVector		foward = rotation.GetForwardVector();
	FVector		addRadius = foward * inCollisionSize;

	inOwner->SetRotation(rotation);
	inOwner->SetLocation(inCurrentLocation);

	mServerDestinationLocation	= inDestinationLocation - addRadius;
	mDestinationLocation		= inDestinationLocation;
	mLastMovementTime			= inMovementLastTime;
}

const Location& MovementComponent::GetServerDestinationLocation() const
{
	return mServerDestinationLocation;
}

const Location& MovementComponent::GetDestinationLocation() const
{
	return mDestinationLocation;
}

const int64 MovementComponent::GetLastMovementTime() const
{
	return mLastMovementTime;
}
