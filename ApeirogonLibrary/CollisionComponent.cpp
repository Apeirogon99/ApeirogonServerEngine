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

void CollisionComponent::SetCollisionType(const CollisionType& inColisionType)
{
	mCollisionType = inColisionType;
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

const CollisionType& CollisionComponent::GetCollisionType() const
{
	return mCollisionType;
}


//==========================//
//	 BoxCollisionComponent	//
//==========================//

BoxCollisionComponent::BoxCollisionComponent() : mBoxCollision(FVector(), FRotator())
{
	this->SetCollisionType(CollisionType::Collision_Box);
}

BoxCollisionComponent::~BoxCollisionComponent()
{
}

void BoxCollisionComponent::SetBoxCollision(const FVector& inBoxExtent)
{
	mBoxCollision.SetBoxExtent(inBoxExtent);
	mBoxCollision.SetOrientation(FRotator());
}

const float BoxCollisionComponent::GetLocalRadius()
{
	return std::sqrtf(std::powf(mBoxCollision.GetBoxExtent().GetX(), 2) + std::powf(mBoxCollision.GetBoxExtent().GetY(), 2));
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
	this->SetCollisionType(CollisionType::Collision_Capsule);
}

CapsuleCollisionComponent::~CapsuleCollisionComponent()
{
}

void CapsuleCollisionComponent::SetCapsuleCollision(const float inRadius, const float inHeight)
{
	mCapsuleCollision.SetRadius(inRadius);
	mCapsuleCollision.SetHeight(inHeight);
}

const float CapsuleCollisionComponent::GetLocalRadius()
{
	return mCapsuleCollision.GetRadius();
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
	this->SetCollisionType(CollisionType::Collision_Sphere);
}

SphereCollisionComponent::~SphereCollisionComponent()
{
}

void SphereCollisionComponent::SetSphereCollisione(const float inRadius)
{
	mSphereCollision.SetRadius(inRadius);
}

const float SphereCollisionComponent::GetLocalRadius()
{
	return mSphereCollision.GetRadius();
}

const SphereCollision& SphereCollisionComponent::GetSphereCollision()
{
	return mSphereCollision;
}