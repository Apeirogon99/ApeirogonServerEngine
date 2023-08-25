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

bool BoxTrace::CollisionTrace(CollisionComponent* inCollision)
{
	bool result = false;

	if (nullptr == inCollision)
	{
		return result;
	}

	const CollisionType& type = inCollision->GetCollisionType();

	switch (type)
	{
	case CollisionType::Collision_Unspecified:
		break;
	case CollisionType::Collision_Box:
		result = BoxCollisionTraceOBB(static_cast<BoxCollisionComponent*>(inCollision));
		break;
	case CollisionType::Collision_Capsule:
		result = CapsuleCollisionTrace(static_cast<CapsuleCollisionComponent*>(inCollision));
		break;
	case CollisionType::Collision_Sphere:
		result = SphereCollisionTrace(static_cast<SphereCollisionComponent*>(inCollision));
		break;
	case CollisionType::Collision_Frustum:
		break;
	default:
		break;
	}

	return result;
}

bool BoxTrace::BoxCollisionTraceAABB(BoxCollisionComponent* inBoxCollisionComponent)
{
	ActorPtr collisionActor = inBoxCollisionComponent->GetOwner().lock();
	if (nullptr == collisionActor)
	{
		return false;
	}

	FVector actorMin;
	FVector actorMax;
	FVector actorCenterLocation = collisionActor->GetLocation();
	inBoxCollisionComponent->GetBoxCollision().MakeAABB(actorCenterLocation, actorMin, actorMax);

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

bool BoxTrace::BoxCollisionTraceOBB(BoxCollisionComponent* inBoxCollisionComponent)
{
	ActorPtr BActor = inBoxCollisionComponent->GetOwner().lock();
	if (nullptr == BActor)
	{
		return false;
	}
	inBoxCollisionComponent->GetBoxCollision().SetOrientation(BActor->GetRotation());

	const Location& BCenter = BActor->GetLocation() + inBoxCollisionComponent->GetLocalLocation();
	const BoxCollision& BBoundBox = inBoxCollisionComponent->GetBoxCollision();
	const FVector BBoxExtent = BBoundBox.GetBoxExtent();

	const Location& ACenter = GetCenterLocation();
	const BoxCollision& ABoundBox = this->mBoxCollision;
	const FVector ABoxExtent = ABoundBox.GetBoxExtent();

	const double cutoff = 0.999999;
	bool existsParallelPair = false;

	FVector D = ACenter - BCenter;

	FVector NAXAxis, NAYAxis, NAZAxis;
	Matrix::GetAxis(ABoundBox.GetOrientation(), NAXAxis, NAYAxis, NAZAxis);

	FVector NBXaxis, NBYaxis, NBZaxis;
	Matrix::GetAxis(BBoundBox.GetOrientation(), NBXaxis, NBYaxis, NBZaxis);

	FVector XAxis(FVector::DotProduct(NAXAxis, NBXaxis), FVector::DotProduct(NAXAxis, NBYaxis), FVector::DotProduct(NAXAxis, NBZaxis));
	FVector YAxis(FVector::DotProduct(NAYAxis, NBXaxis), FVector::DotProduct(NAYAxis, NBYaxis), FVector::DotProduct(NAYAxis, NBZaxis));
	FVector ZAxis(FVector::DotProduct(NAZAxis, NBXaxis), FVector::DotProduct(NAZAxis, NBYaxis), FVector::DotProduct(NAZAxis, NBZaxis));

	if (XAxis.GetX() > cutoff) { existsParallelPair = true; }
	if (XAxis.GetY() > cutoff) { existsParallelPair = true; }
	if (XAxis.GetZ() > cutoff) { existsParallelPair = true; }

	if (YAxis.GetX() > cutoff) { existsParallelPair = true; }
	if (YAxis.GetY() > cutoff) { existsParallelPair = true; }
	if (YAxis.GetZ() > cutoff) { existsParallelPair = true; }

	if (ZAxis.GetX() > cutoff) { existsParallelPair = true; }
	if (ZAxis.GetY() > cutoff) { existsParallelPair = true; }
	if (ZAxis.GetZ() > cutoff) { existsParallelPair = true; }

	float A0 = fabsf(FVector::DotProduct(D, NAXAxis));
	float A1 = fabsf(FVector::DotProduct(D, NAYAxis));
	float A2 = fabsf(FVector::DotProduct(D, NAZAxis));

	float B0 = fabsf(FVector::DotProduct(D, NBXaxis));
	float B1 = fabsf(FVector::DotProduct(D, NBYaxis));
	float B2 = fabsf(FVector::DotProduct(D, NBZaxis));

	//L R0 R1 R
	float R0, R1, R;

	//1 (Ra)x
	R0	= ABoxExtent.GetX();
	R1	= BBoxExtent.GetX() * fabsf(XAxis.GetX()) + BBoxExtent.GetY() * fabsf(XAxis.GetY()) + BBoxExtent.GetZ() * fabsf(XAxis.GetZ());
	R = A0;
	if (R > R0 + R1) { /*printf("(Ra)x        ");*/ return false; }

	//2 (Ra)y
	R0 = ABoxExtent.GetY();
	R1 = BBoxExtent.GetX() * fabsf(YAxis.GetX()) + BBoxExtent.GetY() * fabsf(YAxis.GetY()) + BBoxExtent.GetZ() * fabsf(YAxis.GetZ());
	R = A1;
	if (R > R0 + R1) { /*printf("(Ra)y        ");*/ return false; }


	//3 (Ra)z
	R0 = ABoxExtent.GetY();
	R1 = BBoxExtent.GetX() * fabsf(ZAxis.GetX()) + BBoxExtent.GetY() * fabsf(ZAxis.GetY()) + BBoxExtent.GetZ() * fabsf(ZAxis.GetZ());
	R = A2;
	if (R > R0 + R1) { /*printf("(Ra)z        ");*/ return false; }


	//4 (Rb)x
	R0 = ABoxExtent.GetX() * fabsf(XAxis.GetX()) + ABoxExtent.GetY() * fabsf(YAxis.GetX()) + ABoxExtent.GetZ() * fabsf(ZAxis.GetX());
	R1 = BBoxExtent.GetX();
	R = B0;
	if (R > R0 + R1) { /*printf("(Rb)x        ");*/ return false; }


	//5 (Rb)y
	R0 = ABoxExtent.GetX() * fabsf(XAxis.GetY()) + ABoxExtent.GetY() * fabsf(YAxis.GetY()) + ABoxExtent.GetZ() * fabsf(ZAxis.GetY());
	R1 = BBoxExtent.GetY();
	R = B1;
	if (R > R0 + R1) { /*printf("(Rb)y        ");*/ return false; }


	//6 (Rb)z
	R0 = ABoxExtent.GetX() * fabsf(XAxis.GetZ()) + ABoxExtent.GetY() * fabsf(YAxis.GetZ()) + ABoxExtent.GetZ() * fabsf(ZAxis.GetZ());
	R1 = BBoxExtent.GetZ();
	R = B2;
	if (R > R0 + R1) { /*printf("(Rb)z        ");*/ return false; }

	if (existsParallelPair == true)
	{
		return true;
	}

	//7 (Ra)x X (Rb)x
	R0 = ABoxExtent.GetY() * fabsf(ZAxis.GetX()) + ABoxExtent.GetZ() * fabsf(YAxis.GetX());
	R1 = BBoxExtent.GetY() * fabsf(XAxis.GetZ()) + BBoxExtent.GetZ() * fabsf(XAxis.GetY());
	R = (A2 * YAxis.GetX() - A1 * ZAxis.GetX());
	if (R > R0 + R1) { /*printf("(Ra)x X (Rb)x");*/ return false; }


	//8 (Ra)x X (Rb)y
	R0 = ABoxExtent.GetY() * fabsf(ZAxis.GetY()) + ABoxExtent.GetZ() * fabsf(YAxis.GetZ());
	R1 = BBoxExtent.GetX() * fabsf(XAxis.GetZ()) + BBoxExtent.GetZ() * fabsf(XAxis.GetX());
	R = (A2 * YAxis.GetY() - A1 * ZAxis.GetY());
	if (R > R0 + R1) { /*printf("(Ra)x X (Rb)y");*/ return false; }


	//9 (Ra)x X (Rb)z
	R0 = ABoxExtent.GetY() * fabsf(ZAxis.GetZ()) + ABoxExtent.GetZ() * fabsf(XAxis.GetX());
	R1 = BBoxExtent.GetX() * fabsf(XAxis.GetY()) + BBoxExtent.GetY() * fabsf(XAxis.GetX());
	R = (A2 * YAxis.GetZ() - A1 * ZAxis.GetZ());
	if (R > R0 + R1) { /*printf("(Ra)x X (Rb)z");*/ return false; }


	//10 (Ra)y X (Rb)x
	R0 = ABoxExtent.GetX() * fabsf(ZAxis.GetX()) + ABoxExtent.GetZ() * fabsf(XAxis.GetX());
	R1 = BBoxExtent.GetY() * fabsf(YAxis.GetZ()) + BBoxExtent.GetZ() * fabsf(YAxis.GetY());
	R = (A0 * ZAxis.GetX() - A2 * XAxis.GetX());
	if (R > R0 + R1) { /*printf("(Ra)y X (Rb)x");*/ return false; }


	//11 (Ra)y X (Rb)y
	R0 = ABoxExtent.GetX() * fabsf(ZAxis.GetY()) + ABoxExtent.GetZ() * fabsf(XAxis.GetY());
	R1 = BBoxExtent.GetX() * fabsf(XAxis.GetZ()) + BBoxExtent.GetZ() * fabsf(YAxis.GetX());
	R = (A0 * ZAxis.GetY() - A2 * XAxis.GetY());
	if (R > R0 + R1) { /*printf("(Ra)y X (Rb)y");*/ return false; }


	//12 (Ra)y X (Rb)z
	R0 = ABoxExtent.GetX() * fabsf(ZAxis.GetZ()) + ABoxExtent.GetZ() * fabsf(XAxis.GetZ());
	R1 = BBoxExtent.GetX() * fabsf(YAxis.GetY()) + BBoxExtent.GetY() * fabsf(YAxis.GetX());
	R = (A0 * ZAxis.GetZ() - A2 * XAxis.GetZ());
	if (R > R0 + R1) { /*printf("(Ra)y X (Rb)z");*/ return false; }


	//13 (Ra)z X (Rb)x
	R0 = ABoxExtent.GetX() * fabsf(YAxis.GetX()) + ABoxExtent.GetY() * fabsf(XAxis.GetX());
	R1 = BBoxExtent.GetY() * fabsf(ZAxis.GetZ()) + BBoxExtent.GetZ() * fabsf(ZAxis.GetY());
	R = (A1 * XAxis.GetX() - A0 * YAxis.GetX());
	if (R > R0 + R1) { /*printf("(Ra)z X (Rb)x");*/ return false; }


	//14 (Ra)z X (Rb)y
	R0 = ABoxExtent.GetZ() * fabsf(YAxis.GetY()) + ABoxExtent.GetY() * fabsf(XAxis.GetY());
	R1 = BBoxExtent.GetX() * fabsf(ZAxis.GetZ()) + BBoxExtent.GetZ() * fabsf(ZAxis.GetX());
	R = (A1 * XAxis.GetY() - A0 * YAxis.GetY());
	if (R > R0 + R1) { /*printf("(Ra)z X (Rb)y");*/ return false; }


	//15 (Ra)z X (Rb)z
	R0 = ABoxExtent.GetX() * fabsf(YAxis.GetZ()) + ABoxExtent.GetY() * fabsf(XAxis.GetZ());
	R1 = BBoxExtent.GetX() * fabsf(ZAxis.GetY()) + BBoxExtent.GetY() * fabsf(ZAxis.GetX());
	R = (A1 * XAxis.GetZ() - A0 * YAxis.GetZ());
	if (R > R0 + R1) { /*printf("(Ra)z X (Rb)");*/ return false; }

	return true;
}

bool BoxTrace::CapsuleCollisionTrace(CapsuleCollisionComponent* inCapsuleCollisionComponent)
{
	return false;
}

bool BoxTrace::SphereCollisionTrace(SphereCollisionComponent* inSphereCollisionComponent)
{
	ActorPtr sphereActor = inSphereCollisionComponent->GetOwner().lock();
	if (nullptr == sphereActor)
	{
		return false;
	}
	const FVector	sphereActorLocation = sphereActor->GetLocation();
	const FVector	sphereCenterLocation = sphereActorLocation + inSphereCollisionComponent->GetLocalLocation();
	const float		sphereRadius = inSphereCollisionComponent->GetSphereCollision().GetRadius();

	const FVector boxCenterLocation = this->GetCenterLocation();
	const FVector boxExtent = this->GetBoxCollision().GetBoxExtent();

	FVector boxMax(boxCenterLocation.GetX() + boxExtent.GetX(), boxCenterLocation.GetY() + boxExtent.GetY(), boxCenterLocation.GetZ() + boxExtent.GetZ());
	FVector boxMin(boxCenterLocation.GetX() - boxExtent.GetX(), boxCenterLocation.GetY() - boxExtent.GetY(), boxCenterLocation.GetZ() - boxExtent.GetZ());

	FVector closeLocation;
	closeLocation.SetX(std::fmax(boxMin.GetX(), std::fmin(this->mStart.GetX(), boxMax.GetX())));
	closeLocation.SetY(std::fmax(boxMin.GetY(), std::fmin(this->mStart.GetY(), boxMax.GetY())));
	closeLocation.SetZ(std::fmax(boxMin.GetZ(), std::fmin(this->mStart.GetZ(), boxMax.GetZ())));

	float distance = FVector::Distance(this->mStart, closeLocation);

	return distance <= sphereRadius;
}

FVector BoxTrace::GetCenterLocation() const
{
	return (mStart + mEnd) / 2.0f;
}

const BoxCollision& BoxTrace::GetBoxCollision() const
{
	return mBoxCollision;
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

bool CapsuleTrace::CollisionTrace(CollisionComponent* inCollision)
{
	return false;
}

bool CapsuleTrace::BoxCollisionTraceAABB(BoxCollisionComponent* inBoxCollisionComponent)
{
	return false;
}

bool CapsuleTrace::BoxCollisionTraceOBB(BoxCollisionComponent* inBoxCollisionComponent)
{
	return false;
}

bool CapsuleTrace::CapsuleCollisionTrace(CapsuleCollisionComponent* inCapsuleCollisionComponent)
{
	return false;
}

bool CapsuleTrace::SphereCollisionTrace(SphereCollisionComponent* inSphereCollisionComponent)
{
	return false;
}

FVector CapsuleTrace::GetCenterLocation() const
{
	return mStart;
}

const CapsuleCollision& CapsuleTrace::GetCapsuleCollision() const
{
	return mCapsuleCollision;
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

bool SphereTrace::CollisionTrace(CollisionComponent* inCollision)
{
	bool result = false;

	if (nullptr == inCollision)
	{
		return result;
	}

	const CollisionType& type = inCollision->GetCollisionType();

	switch (type)
	{
	case CollisionType::Collision_Unspecified:
		break;
	case CollisionType::Collision_Box:
		result = BoxCollisionTraceOBB(static_cast<BoxCollisionComponent*>(inCollision));
		break;
	case CollisionType::Collision_Capsule:
		result = CapsuleCollisionTrace(static_cast<CapsuleCollisionComponent*>(inCollision));
		break;
	case CollisionType::Collision_Sphere:
		result = SphereCollisionTrace(static_cast<SphereCollisionComponent*>(inCollision));
		break;
	case CollisionType::Collision_Frustum:
		break;
	default:
		break;
	}

	return result;
}

bool SphereTrace::BoxCollisionTraceAABB(BoxCollisionComponent* inBoxCollisionComponent)
{
	ActorPtr boxActor = inBoxCollisionComponent->GetOwner().lock();
	if (nullptr == boxActor)
	{
		return false;
	}
	Location actorLocation = boxActor->GetLocation();
	Location boxCenterLocation = actorLocation + inBoxCollisionComponent->GetLocalLocation();

	FVector boxMin;
	FVector boxMax;
	inBoxCollisionComponent->GetBoxCollision().MakeAABB(boxCenterLocation, boxMin, boxMax);

	FVector closeLocation;
	closeLocation.SetX(std::fmax(boxMin.GetX(), std::fmin(this->mStart.GetX(), boxMax.GetX())));
	closeLocation.SetY(std::fmax(boxMin.GetY(), std::fmin(this->mStart.GetY(), boxMax.GetY())));
	closeLocation.SetZ(std::fmax(boxMin.GetZ(), std::fmin(this->mStart.GetZ(), boxMax.GetZ())));

	float distance = FVector::Distance(this->mStart, closeLocation);

	return distance <= this->mSphereCollision.GetRadius();
}

bool SphereTrace::BoxCollisionTraceOBB(BoxCollisionComponent* inBoxCollisionComponent)
{
	ActorPtr boxActor = inBoxCollisionComponent->GetOwner().lock();
	if (nullptr == boxActor)
	{
		return false;
	}
	const FVector	boxActorLocation	= boxActor->GetLocation();
	const FVector	boxCenterLocation	= boxActorLocation + inBoxCollisionComponent->GetLocalLocation();
	const FVector	boxExtent			= inBoxCollisionComponent->GetBoxCollision().GetBoxExtent();

	const float		sphereRadius			= this->GetSphereCollision().GetRadius();
	const FVector	sphereCenterLocation	= this->GetCenterLocation();


	FVector boxMax(boxCenterLocation.GetX() + boxExtent.GetX(), boxCenterLocation.GetY() + boxExtent.GetY(), boxCenterLocation.GetZ() + boxExtent.GetZ());
	FVector boxMin(boxCenterLocation.GetX() - boxExtent.GetX(), boxCenterLocation.GetY() - boxExtent.GetY(), boxCenterLocation.GetZ() - boxExtent.GetZ());

	FVector closeLocation;
	closeLocation.SetX(std::fmax(boxMin.GetX(), std::fmin(this->mStart.GetX(), boxMax.GetX())));
	closeLocation.SetY(std::fmax(boxMin.GetY(), std::fmin(this->mStart.GetY(), boxMax.GetY())));
	closeLocation.SetZ(std::fmax(boxMin.GetZ(), std::fmin(this->mStart.GetZ(), boxMax.GetZ())));

	float distance = FVector::Distance(this->mStart, closeLocation);

	return distance <= this->mSphereCollision.GetRadius();
}

bool SphereTrace::CapsuleCollisionTrace(CapsuleCollisionComponent* inCapsuleCollisionComponent)
{
	return false;
}

bool SphereTrace::SphereCollisionTrace(SphereCollisionComponent* inSphereCollisionComponent)
{
	ActorPtr otherSphereActor = inSphereCollisionComponent->GetOwner().lock();
	if (nullptr == otherSphereActor)
	{
		return false;
	}
	Location otherSphereActorLocation = otherSphereActor->GetLocation();
	Location otherSphereCenterLocation = otherSphereActorLocation + inSphereCollisionComponent->GetLocalLocation();
	Location thisSphereCenterLocation = this->mStart;

	float otherRadius = inSphereCollisionComponent->GetSphereCollision().GetRadius();
	float thisRadius = this->mSphereCollision.GetRadius();
	float maxRadius	= otherRadius + thisRadius;

	float distance = FVector::Distance(otherSphereCenterLocation, thisSphereCenterLocation);
	return (distance <= maxRadius) ? true : false;
}

FVector SphereTrace::GetCenterLocation() const
{
	return mStart;
}

const SphereCollision& SphereTrace::GetSphereCollision() const
{
	return mSphereCollision;
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

bool FrustumTrace::CollisionTrace(CollisionComponent* inCollision)
{
	return false;
}


bool FrustumTrace::BoxCollisionTraceAABB(BoxCollisionComponent* inBoxCollisionComponent)
{
	return false;
}

bool FrustumTrace::BoxCollisionTraceOBB(BoxCollisionComponent* inBoxCollisionComponent)
{
	return false;
}

bool FrustumTrace::CapsuleCollisionTrace(CapsuleCollisionComponent* inCapsuleCollisionComponent)
{
	return false;
}

bool FrustumTrace::SphereCollisionTrace(SphereCollisionComponent* inSphereCollisionComponent)
{
	return false;
}