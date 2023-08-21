#include "pch.h"
#include "Collision.h"

//==========================//
//		 BoxCollision		//
//==========================//

BoxCollision::BoxCollision(const FVector& inBoxExtent, const FRotator& inOrientation) : Collision(CollisionType::Collision_Box), mExtent(inBoxExtent), mOrientation(inOrientation)
{
}

BoxCollision::~BoxCollision()
{
}

void BoxCollision::MakeAABB(const FVector& inCenterLocation, FVector& outMin, FVector& outMax)
{
    outMin = FVector(inCenterLocation.GetX() - this->mExtent.GetX(), inCenterLocation.GetY() - this->mExtent.GetY(), inCenterLocation.GetZ() - this->mExtent.GetZ());
    outMax = FVector(inCenterLocation.GetX() + this->mExtent.GetX(), inCenterLocation.GetY() + this->mExtent.GetY(), inCenterLocation.GetZ() + this->mExtent.GetZ());
}

void BoxCollision::MakeOBB(const FVector& inCenterLocation, FVector& outMin, FVector& outMax)
{
}

bool BoxCollision::BoxCollisionCheck(const BoxCollision& inBoxCollision)
{
    return false;
}

bool BoxCollision::CapsuleCollisionCheck(const CapsuleCollision& inCapsuleCollision)
{
    return false;
}

bool BoxCollision::SphereCollisionCheck(const SphereCollision& inSphereCollision)
{
    return false;
}

bool BoxCollision::FrustumCollisionCheck(const FrustumCollision& inFrustumCollision)
{
    return false;
}

//==========================//
//	   CapsuleCollision		//
//==========================//

CapsuleCollision::CapsuleCollision(const float inRadius, const float inHeight) : Collision(CollisionType::Collision_Capsule), mRadius(inRadius), mHeight(inHeight)
{
}

CapsuleCollision::~CapsuleCollision()
{
}

bool CapsuleCollision::BoxCollisionCheck(const BoxCollision& inBoxCollision)
{
    return false;
}

bool CapsuleCollision::CapsuleCollisionCheck(const CapsuleCollision& inCapsuleCollision)
{
    return false;
}

bool CapsuleCollision::SphereCollisionCheck(const SphereCollision& inSphereCollision)
{
    return false;
}

bool CapsuleCollision::FrustumCollisionCheck(const FrustumCollision& inFrustumCollision)
{
    return false;
}

//==========================//
//	    SphereCollision		//
//==========================//

SphereCollision::SphereCollision(const float inRadius) : Collision(CollisionType::Collision_Sphere), mRadius(inRadius)
{
}

SphereCollision::~SphereCollision()
{
}

bool SphereCollision::BoxCollisionCheck(const BoxCollision& inBoxCollision)
{
    return false;
}

bool SphereCollision::CapsuleCollisionCheck(const CapsuleCollision& inCapsuleCollision)
{
    return false;
}

bool SphereCollision::SphereCollisionCheck(const SphereCollision& inSphereCollision)
{
    return false;
}

bool SphereCollision::FrustumCollisionCheck(const FrustumCollision& inFrustumCollision)
{
    return false;
}

//==========================//
//	   FrustumCollision		//
//==========================//

FrustumCollision::FrustumCollision(const float inNearPlane, const float inFarPlane, const float inHeight) : Collision(CollisionType::Collision_Frustum), mNearPlane(inNearPlane), mFarPlane(inFarPlane), mHeight(inHeight)
{
}

FrustumCollision::~FrustumCollision()
{
}

bool FrustumCollision::BoxCollisionCheck(const BoxCollision& inBoxCollision)
{
    return false;
}

bool FrustumCollision::CapsuleCollisionCheck(const CapsuleCollision& inCapsuleCollision)
{
    return false;
}

bool FrustumCollision::SphereCollisionCheck(const SphereCollision& inSphereCollision)
{
    return false;
}

bool FrustumCollision::FrustumCollisionCheck(const FrustumCollision& inFrustumCollision)
{
    return false;
}
