#include "pch.h"
#include "CollisionComponent.h"

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