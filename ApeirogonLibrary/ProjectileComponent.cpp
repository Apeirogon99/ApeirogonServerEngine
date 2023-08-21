#include "pch.h"
#include "ProjectileComponent.h"

ProjectileComponent::ProjectileComponent() : mLocation(), mRotation(), mMaxSyncTime(0), mLastMovementTime(0), mCurrentMovementSyncTime(0)
{
}

ProjectileComponent::~ProjectileComponent()
{
}

void ProjectileComponent::InitProjectile(const Location& inInitLocation, const Rotation& inInitRotation, const int64& inMaxSyncTime, const int64& inWorldTime)
{
	mLocation = inInitLocation;
	mRotation = inInitRotation;
	mMaxSyncTime = inMaxSyncTime;
	mLastMovementTime = inWorldTime;
	mCurrentMovementSyncTime = 0;
}

bool ProjectileComponent::Update(ActorPtr inOwner)
{
	Location currentLocation = inOwner->GetLocation();

	Velocity	currentVelocity = inOwner->GetVelocity();
	const int64 currentWorldTime = inOwner->GetWorld().lock()->GetWorldTime();
	const float	duration = static_cast<float>(currentWorldTime - this->mLastMovementTime) / 1000.0f;

	FVector	direction = this->mRotation.Vector();
	FVector velocity = direction * currentVelocity;
	FVector	deadReckoningLocation = currentLocation + (velocity * duration);

	inOwner->SetLocation(deadReckoningLocation);
	this->mLastMovementTime = currentWorldTime;
	return true;
}

bool ProjectileComponent::SyncUpdate(ActorPtr inOwner, const int64 inSyncTime)
{
	mCurrentMovementSyncTime += inSyncTime;
	if (mCurrentMovementSyncTime <= mMaxSyncTime)
	{
		return false;
	}
	mCurrentMovementSyncTime = 0;

	return true;
}

const Location ProjectileComponent::GetCurrentLocation(ActorPtr inOwner)
{
	Location currentLocation = inOwner->GetLocation();

	Velocity	currentVelocity = inOwner->GetVelocity();
	const int64 currentWorldTime = inOwner->GetWorld().lock()->GetWorldTime();
	const float	duration = static_cast<float>(currentWorldTime - this->mLastMovementTime) / 1000.0f;

	FVector	direction = this->mRotation.Vector();
	FVector velocity = direction * currentVelocity;
	FVector	deadReckoningLocation = currentLocation + (velocity * duration);

	return deadReckoningLocation;
}

const int64 ProjectileComponent::GetLastMovementTime() const
{
	return mLastMovementTime;
}
