#include "pch.h"
#include "CollisionComponent.h"

//==========================//
//	  CollisionComponent	//
//==========================//

void CollisionComponent::InitCollisionComponent(ActorRef inOwner, const FVector& inLocalLocation, const FRotator& inLocalRotation)
{
	mOwner = inOwner;
	mLocalLocation = inLocalLocation;
	mLocalRotation = inLocalRotation;
}

void CollisionComponent::SetOwner(ActorRef inOwner)
{
	mOwner = inOwner;
}

void CollisionComponent::SetLocalLocation(const FVector& inLocalLocation)
{
	mLocalLocation = inLocalLocation;
}

void CollisionComponent::SetLocalRotation(const FRotator& inLocalRotation)
{
	mLocalRotation = inLocalRotation;
}

const ActorRef& CollisionComponent::GetOwner() const
{
	return mOwner;
}

const FVector& CollisionComponent::GetLocalLocation() const
{
	return mLocalLocation;
}

const FRotator& CollisionComponent::GetLocalRotation() const
{
	return mLocalRotation;
}


//==========================//
//	 BoxCollisionComponent	//
//==========================//

BoxCollisionComponent::BoxCollisionComponent() : mBoxCollision(FVector(), FRotator())
{
}

BoxCollisionComponent::~BoxCollisionComponent()
{
}

void BoxCollisionComponent::SetBoxCollision(const FVector& inBoxExtent)
{
	mBoxCollision.SetBoxExtent(inBoxExtent);
	mBoxCollision.SetOrientation(FRotator());
}

BoxCollision& BoxCollisionComponent::GetBoxCollision()
{
	return mBoxCollision;
}

//===========================//
// CapsuleCollisionComponent //
//===========================//

CapsuleCollisionComponent::CapsuleCollisionComponent() : mCapsuleCollision(0.0f, 0.0f)
{
}

CapsuleCollisionComponent::~CapsuleCollisionComponent()
{
}

void CapsuleCollisionComponent::SetCapsuleCollision(const float inRadius, const float inHeight)
{
	mCapsuleCollision.SetRadius(inRadius);
	mCapsuleCollision.SetHeight(inHeight);
}

const CapsuleCollision& CapsuleCollisionComponent::GetCapsuleCollision()
{
	return mCapsuleCollision;
}

//==========================//
// SphereCollisionComponent //
//==========================//

SphereCollisionComponent::SphereCollisionComponent() : mSphereCollision(0.0f)
{
}

SphereCollisionComponent::~SphereCollisionComponent()
{
}

void SphereCollisionComponent::SetSphereCollisione(const float inRadius)
{
	mSphereCollision.SetRadius(inRadius);
}

const SphereCollision& SphereCollisionComponent::GetSphereCollision()
{
	return mSphereCollision;
}