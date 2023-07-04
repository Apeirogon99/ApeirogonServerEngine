#include "pch.h"
#include "CollisionComponent.h"

BoxCollisionComponent::BoxCollisionComponent() : mBoxCollision(FVector(), FRotator())
{
}

BoxCollisionComponent::~BoxCollisionComponent()
{
}

CapsuleCollisionComponent::CapsuleCollisionComponent() : mCapsuleCollision(0.0f, 0.0f)
{
}

CapsuleCollisionComponent::~CapsuleCollisionComponent()
{
}

void CapsuleCollisionComponent::SetCapsule(const float inRadius, const float inHeight)
{
	mCapsuleCollision.SetRadius(inRadius);
	mCapsuleCollision.SetHeight(inHeight);
}

SphereCollisionComponent::SphereCollisionComponent() : mSphereCollision(0.0f)
{
}

SphereCollisionComponent::~SphereCollisionComponent()
{
}