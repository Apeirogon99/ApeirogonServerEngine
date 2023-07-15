#include "pch.h"
#include "Trace.h"
#include "CollisionComponent.h"

bool Trace::HitCollision(Collision& inCollision)
{
	return false;
}

const ETraceType& Trace::GetTraceType() const
{
	return mTraceType;
}

//==========================//
//		   BoxTrace			//
//==========================//

BoxTrace::BoxTrace(FVector inStart, FVector inEnd, bool inIsIgnore, FVector inBoxExtent, FRotator inOrientation) : Trace(inStart, inEnd, inIsIgnore, ETraceType::Trace_Box), mBoxCollision(inBoxExtent, inOrientation)
{
}

bool BoxTrace::BoxCollisionTrace(BoxCollisionComponent& inBoxCollisionComponent)
{
	ActorPtr collisionActor = inBoxCollisionComponent.GetOwner().lock();
	if (nullptr == collisionActor)
	{
		return false;
	}
	Location currentLocation = collisionActor->GetLocation();

	FVector actorMin;
	FVector actorMax;
	inBoxCollisionComponent.GetBoxCollision().MakeAABB(currentLocation, actorMin, actorMax);

	FVector traceMin;
	FVector traceMax;
	this->mBoxCollision.MakeAABB(mStart, traceMin, traceMax);

	if (traceMin.GetX() < actorMax.GetX() && traceMax.GetX() > actorMin.GetX() &&
		traceMin.GetY() < actorMax.GetY() && traceMax.GetY() > actorMin.GetY() &&
		traceMin.GetZ() < actorMax.GetZ() && traceMax.GetZ() > actorMin.GetZ())
	{
		wprintf(L"TRACE\n");
		return true;
	}

	return false;
}

bool BoxTrace::CapsuleCollisionTrace(const CapsuleCollision& inCapsuleCollision)
{
	return true;
}

bool BoxTrace::SphereCollisionTrace(const SphereCollision& inSphereCollision)
{
	return true;
}

bool BoxTrace::FrustumCollisionTrace(const FrustumCollision& inFrustumCollision)
{
	return true;
}

//==========================//
//		 CapsuleTrace		//
//==========================//