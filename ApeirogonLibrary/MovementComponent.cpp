#include "pch.h"
#include "MovementComponent.h"

MovementComponent::MovementComponent() : mDestinationLocation(), mOldLocation(), mServerDestinationLocation(), mLastMovementTime(0), mCurrentMovementSyncTime(0), mMaxMovementSyncTime(0), mIsRestrict(false)
{
}

MovementComponent::~MovementComponent()
{
}

void MovementComponent::InitMovement(const Location& inInitLocation, const int64& inMovementMaxSyncTime, const int64& inWorldTime)
{
	mDestinationLocation		= inInitLocation;
	mServerDestinationLocation	= inInitLocation;
	mOldLocation				= inInitLocation;
	mLastMovementTime			= inWorldTime;
	mMaxMovementSyncTime		= inMovementMaxSyncTime;
	mCurrentMovementSyncTime	= 0;
}

bool MovementComponent::Update(ActorPtr inOwner, const float& inCloseToDestination)
{

	const int64 currentWorldTime = inOwner->GetWorld().lock()->GetWorldTime();

	Location currentLocation = inOwner->GetLocation();
	Location destinationLocation = this->mServerDestinationLocation;

	if (FVector::Comapre(currentLocation, destinationLocation))
	{
		return false;
	}

	const float locationDistance = FVector::Distance2D(currentLocation, destinationLocation);
	if (locationDistance <= inCloseToDestination)
	{
		inOwner->SetLocation(destinationLocation);
		return true;
	}

	Velocity	currentVelocity		= inOwner->GetVelocity();

	const float	duration			= static_cast<float>(currentWorldTime - this->mLastMovementTime) / 1000.0f;

	FVector		direction	= destinationLocation - currentLocation;
	FRotator	rotation	= inOwner->GetRotation();

	FVector		foward		= rotation.GetForwardVector();
	FVector		velocity	= foward * currentVelocity;

	FVector	deadReckoningLocation = currentLocation + (velocity * duration);

	inOwner->SetLocation(deadReckoningLocation);
	this->mOldLocation = currentLocation;
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
	if (distance <= 1.0f)
	{
		return false;
	}

	mOldLocation = inOwner->GetLocation();
	return true;
}

void MovementComponent::SetRestrictMovement(bool inRestrict)
{
	mIsRestrict = inRestrict;
}

void MovementComponent::SetNewDestination(ActorPtr inOwner, const Location& inCurrentLocation, const Location& inDestinationLocation, const int64 inMovementLastTime, const float& inCollisionSize)
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

void MovementComponent::SetTeleportDestination(ActorPtr inOwner, const Location& inDestinationLocation, const int64 inMovementLastTime)
{
}

const Location MovementComponent::GetCurrentLocation(ActorPtr inOwner)
{
	WorldPtr world = inOwner->GetWorld().lock();
	if (nullptr == world)
	{
		assert(!world);
	}
	const int64 worldTime = world->GetWorldTime();

	Location currentLocation		= inOwner->GetLocation();
	Location destinationLocation	= this->mServerDestinationLocation;

	if (FVector::Comapre(currentLocation, destinationLocation))
	{
		return currentLocation;
	}

	Velocity	currentVelocity		= inOwner->GetVelocity();
	const float	duration = static_cast<float>(worldTime - this->mLastMovementTime) / 1000.0f;

	FVector	direction = (destinationLocation - currentLocation).Normalize();
	FVector velocity = direction * currentVelocity;

	FVector	deadReckoningLocation = currentLocation + (velocity * duration);

	return deadReckoningLocation;
}

const Location MovementComponent::GetNextLocation(ActorPtr inOwner)
{
	WorldPtr world = inOwner->GetWorld().lock();
	if (nullptr == world)
	{
		assert(!world);
	}
	const int64 worldTime = world->GetNextWorldTime();

	Location currentLocation = inOwner->GetLocation();
	Location destinationLocation = this->mServerDestinationLocation;

	if (FVector::Comapre(currentLocation, destinationLocation))
	{
		return currentLocation;
	}

	Velocity	currentVelocity = inOwner->GetVelocity();
	const float	duration = static_cast<float>(worldTime - this->mLastMovementTime) / 1000.0f;

	FVector	direction = (destinationLocation - currentLocation).Normalize();
	FVector velocity = direction * currentVelocity;

	FVector	deadReckoningLocation = currentLocation + (velocity * duration);

	return deadReckoningLocation;
}

const Location MovementComponent::GetNextLocation(ActorPtr inOwner, const Location& inCurrentLocation, const Location& inDestinationLocation, const int64& inMovementLastTime, float inRadius)
{
	Location currentLocation = inCurrentLocation;
	Location destinationLocation = inDestinationLocation;

	if (1.0f >= FVector::Distance2D(currentLocation, destinationLocation))
	{
		return currentLocation;
	}

	Velocity currentVelocity = inOwner->GetVelocity();
	const float	duration = static_cast<float>(inMovementLastTime) / 1000.0f;

	FVector	direction = (destinationLocation - currentLocation).Normalize();
	FVector velocity = direction * currentVelocity;

	FVector	deadReckoningLocation = currentLocation + (velocity * duration) - (direction * inRadius);

	return deadReckoningLocation;
}

const Location& MovementComponent::GetOldLocation() const
{
	return mOldLocation;
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

const bool MovementComponent::GetRestrictMovement() const
{
	return mIsRestrict;
}
