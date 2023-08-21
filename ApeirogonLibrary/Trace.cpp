#include "pch.h"
#include "Trace.h"

Trace::Trace(ActorRef inOwner, FVector inStart, FVector inEnd, bool inIsIgnore, ETraceType inType) : mOwner(inOwner), mStart(inStart), mEnd(inEnd), mIsIgnore(inIsIgnore), mTraceType(inType)
{
}

const ActorRef& Trace::GetOwner() const
{
	return mOwner;
}

const ETraceType& Trace::GetTraceType() const
{
	return mTraceType;
}

//==========================//
//		   BoxTrace			//
//==========================//

BoxTrace::BoxTrace(ActorRef inOwner, FVector inStart, FVector inEnd, bool inIsIgnore, FVector inBoxExtent, FRotator inOrientation) : Trace(inOwner, inStart, inEnd, inIsIgnore, ETraceType::Trace_Box), mBoxCollision(inBoxExtent, inOrientation)
{
}

bool BoxTrace::BoxCollisionTrace(BoxCollisionComponent& inBoxCollisionComponent)
{
	ActorPtr collisionActor = inBoxCollisionComponent.GetOwner().lock();
	if (nullptr == collisionActor)
	{
		return false;
	}

	FVector actorMin;
	FVector actorMax;
	FVector actorCenterLocation = collisionActor->GetLocation();
	inBoxCollisionComponent.GetBoxCollision().MakeAABB(actorCenterLocation, actorMin, actorMax);

	FVector traceMin;
	FVector traceMax;
	FVector traceCenterLocation = this->GetCenterLocation();
	this->mBoxCollision.MakeAABB(traceCenterLocation, traceMin, traceMax);

	if (traceMin.GetX() < actorMax.GetX() && traceMax.GetX() > actorMin.GetX() &&
		traceMin.GetY() < actorMax.GetY() && traceMax.GetY() > actorMin.GetY() &&
		traceMin.GetZ() < actorMax.GetZ() && traceMax.GetZ() > actorMin.GetZ())
	{
		return true;
	}

	return false;
}

bool BoxTrace::BoxCollisionTraceOBB(BoxCollisionComponent& inBoxCollisionComponent)
{
	ActorPtr collisionActor = inBoxCollisionComponent.GetOwner().lock();
	if (nullptr == collisionActor)
	{
		return false;
	}

	FRotator rotation = this->mBoxCollision.GetOrientation();
	//Matrix matrix = ;

	//행렬을 알아야함
	//분리축을 검사(15개) 해야함
	//그중에 2개가 안맞으면 탈락
	//그리고 2개이상이 된다면 분리축 설정하고
	//충돌하였는지 확인
	//양수라면 충돌 음수라면 충돌 안함

	return false;
}

bool BoxTrace::CapsuleCollisionTrace(CapsuleCollisionComponent& inCapsuleCollisionComponent)
{
	return false;
}

bool BoxTrace::SphereCollisionTrace(SphereCollisionComponent& inSphereCollisionComponent)
{
	ActorPtr sphereActor = inSphereCollisionComponent.GetOwner().lock();
	if (nullptr == sphereActor)
	{
		return false;
	}
	FVector sphereActorLocation = sphereActor->GetLocation();

	const float		sphereRadius = inSphereCollisionComponent.GetSphereCollision().GetRadius();
	const FVector	sphereCenterLocation = sphereActorLocation + inSphereCollisionComponent.GetLocalLocation();

	const FVector	boxCenterLocation = this->GetCenterLocation();

	const FVector	newSphereCenterLocation = sphereCenterLocation - boxCenterLocation;

	return false;
}

FVector BoxTrace::GetCenterLocation() const
{
	return (mStart + mEnd) / 2.0f;
}

//==========================//
//		 CapsuleTrace		//
//==========================//
CapsuleTrace::CapsuleTrace(ActorRef inOwner, FVector inStart, FVector inEnd, bool inIsIgnore, const float inRadius, const float inHeight) : Trace(inOwner, inStart, inEnd, inIsIgnore, ETraceType::Trace_Capsule), mCapsuleCollision(inRadius, inHeight)
{

}

CapsuleTrace::~CapsuleTrace()
{

}

bool CapsuleTrace::BoxCollisionTrace(BoxCollisionComponent& inBoxCollisionComponent)
{
	return false;
}

bool CapsuleTrace::CapsuleCollisionTrace(CapsuleCollisionComponent& inCapsuleCollisionComponent)
{
	return false;
}

bool CapsuleTrace::SphereCollisionTrace(SphereCollisionComponent& inSphereCollisionComponent)
{
	return false;
}

//==========================//
//		 SphereTrace		//
//==========================//
SphereTrace::SphereTrace(ActorRef inOwner, FVector inLocation, bool inIsIgnore, float inRadius) : Trace(inOwner, inLocation, inLocation, inIsIgnore, ETraceType::Trace_Sphere), mSphereCollision(inRadius)
{

}

SphereTrace::~SphereTrace()
{

}

bool SphereTrace::BoxCollisionTrace(BoxCollisionComponent& inBoxCollisionComponent)
{
	ActorPtr boxActor = inBoxCollisionComponent.GetOwner().lock();
	if (nullptr == boxActor)
	{
		return false;
	}
	Location actorLocation = boxActor->GetLocation();
	Location boxCenterLocation = actorLocation + inBoxCollisionComponent.GetLocalLocation();

	FVector boxMin;
	FVector boxMax;
	inBoxCollisionComponent.GetBoxCollision().MakeAABB(boxCenterLocation, boxMin, boxMax);

	FVector closeLocation;
	closeLocation.SetX(std::fmax(boxMin.GetX(), std::fmin(this->mStart.GetX(), boxMax.GetX())));
	closeLocation.SetY(std::fmax(boxMin.GetY(), std::fmin(this->mStart.GetY(), boxMax.GetY())));
	closeLocation.SetZ(std::fmax(boxMin.GetZ(), std::fmin(this->mStart.GetZ(), boxMax.GetZ())));

	float distance = FVector::Distance(this->mStart, closeLocation);

	return distance <= this->mSphereCollision.GetRadius();
}

bool SphereTrace::CapsuleCollisionTrace(CapsuleCollisionComponent& inCapsuleCollisionComponent)
{
	return false;
}

bool SphereTrace::SphereCollisionTrace(SphereCollisionComponent& inSphereCollisionComponent)
{
	ActorPtr otherSphereActor = inSphereCollisionComponent.GetOwner().lock();
	if (nullptr == otherSphereActor)
	{
		return false;
	}
	Location otherSphereActorLocation = otherSphereActor->GetLocation();
	Location otherSphereCenterLocation = otherSphereActorLocation + inSphereCollisionComponent.GetLocalLocation();
	Location thisSphereCenterLocation = this->mStart;

	float otherRadius = inSphereCollisionComponent.GetSphereCollision().GetRadius();
	float thisRadius = this->mSphereCollision.GetRadius();
	float maxRadius	= otherRadius + thisRadius;

	float distance = FVector::Distance(otherSphereCenterLocation, thisSphereCenterLocation);
	return (distance <= maxRadius) ? true : false;
}

//==========================//
//		 FrustmTrace		//
//==========================//
FrustumTrace::FrustumTrace(ActorRef inOwner, FVector inStart, FVector inEnd, bool inIsIgnore, float inNearPlane, float inFarPlane, float inHeight) : Trace(inOwner, inStart, inEnd, inIsIgnore, ETraceType::Trace_Frustum), mFrustumCollision(inNearPlane, inFarPlane, inHeight)
{

}

FrustumTrace::~FrustumTrace()
{

}

bool FrustumTrace::BoxCollisionTrace(BoxCollisionComponent& inBoxCollisionComponent)
{
	return false;
}

bool FrustumTrace::CapsuleCollisionTrace(CapsuleCollisionComponent& inCapsuleCollisionComponent)
{
	return false;
}

bool FrustumTrace::SphereCollisionTrace(SphereCollisionComponent& inSphereCollisionComponent)
{
	return false;
}