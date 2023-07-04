#include "pch.h"
#include "MovementComponent.h"

MovementComponent::MovementComponent() : mLastMovementLocation(0)
{
}

MovementComponent::~MovementComponent()
{
}

void MovementComponent::SetNewDestination(const Location& inDestinationLocation)
{
	mDestinationLocation = inDestinationLocation;
}

bool MovementComponent::Update(ActorPtr& inOwner, const float inCloseToDestination)
{

	Location	currentLocation = inOwner->GetLocation();
	Location	destinationLocation = this->mDestinationLocation;

	const float locationDistance = FVector::Distance2D(currentLocation, destinationLocation);
	if (locationDistance > inCloseToDestination)
	{
		return false;
	}

	Velocity	currentVelocity		= inOwner->GetVelocity();
	const int64 currentWorldTime	= inOwner->GetWorld().lock()->GetWorldTime();
	const float	duration			= static_cast<float>(currentWorldTime - this->mLastMovementLocation) / 1000.0f;

	FVector	direction = destinationLocation - currentLocation;
	direction.Normalize();

	FRotator rotation = direction.Rotator();
	FVector velocity = direction * currentVelocity;

	FVector	deadReckoningLocation = currentLocation + (velocity * duration);

	//inOwner->GameObjectLog(L"[Move] (%5.6f:%5.6f:%5.6f)\n", deadReckoningLocation.GetX(), deadReckoningLocation.GetY(), deadReckoningLocation.GetZ());

	inOwner->SetLocation(deadReckoningLocation);
	inOwner->SetRotation(rotation);
	this->mLastMovementLocation = currentWorldTime;
	return true;
}
