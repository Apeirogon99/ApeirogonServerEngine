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

bool BoxTrace::BoxCollisionTraceAABB(BoxCollisionComponent& inBoxCollisionComponent)
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
	ActorPtr BActor = inBoxCollisionComponent.GetOwner().lock();
	if (nullptr == BActor)
	{
		return false;
	}
	inBoxCollisionComponent.GetBoxCollision().SetOrientation(BActor->GetRotation());

	Matrix OBMatrix;
	Matrix OBInveMatrix;
	const Location& BCenter = BActor->GetLocation() + inBoxCollisionComponent.GetLocalLocation();
	const BoxCollision& BBoundBox = inBoxCollisionComponent.GetBoxCollision();
	const FVector BBoxExtent = BBoundBox.GetBoxExtent();
	inBoxCollisionComponent.GetBoxCollision().MakeOBB(BCenter, OBMatrix, OBInveMatrix);

	Matrix OAMatrix;
	Matrix OAInvMatrix;
	const Location& ACenter = GetCenterLocation();
	const BoxCollision& ABoundBox = this->mBoxCollision;
	const FVector ABoxExtent = ABoundBox.GetBoxExtent();
	this->mBoxCollision.MakeOBB(ACenter, OAMatrix, OAInvMatrix);

	////Trace를 기준 좌표계로 변환
	Matrix	mat = OBMatrix * OAInvMatrix;
	FVector pos = OAInvMatrix * (BCenter - ACenter);
	// 
	//축 


	FVector D = ACenter - BCenter;

	Matrix	AMatrix = Matrix::RotateMatrix(ACenter, ABoundBox.GetOrientation());
	FVector AXaxis(AMatrix.m00, AMatrix.m10, AMatrix.m20);
	FVector AYaxis(AMatrix.m01, AMatrix.m11, AMatrix.m21);
	FVector AZaxis(AMatrix.m02, AMatrix.m12, AMatrix.m22);

	Matrix	BMatrix = Matrix::RotateMatrix(BCenter, BBoundBox.GetOrientation());
	FVector BXaxis(BMatrix.m00, BMatrix.m10, BMatrix.m20);
	FVector BYaxis(BMatrix.m01, BMatrix.m11, BMatrix.m21);
	FVector BZaxis(BMatrix.m02, BMatrix.m12, BMatrix.m22);

	FVector XAxis(FVector::DotProduct(AXaxis, BXaxis), FVector::DotProduct(AXaxis, BYaxis), FVector::DotProduct(AXaxis, BZaxis));
	FVector YAxis(FVector::DotProduct(AYaxis, BXaxis), FVector::DotProduct(AYaxis, BYaxis), FVector::DotProduct(AYaxis, BZaxis));
	FVector ZAxis(FVector::DotProduct(AZaxis, BXaxis), FVector::DotProduct(AZaxis, BYaxis), FVector::DotProduct(AZaxis, BZaxis));

	float A0 = fabsf(FVector::DotProduct(D, AXaxis));
	float A1 = fabsf(FVector::DotProduct(D, AYaxis));
	float A2 = fabsf(FVector::DotProduct(D, AZaxis));

	float B0 = fabsf(FVector::DotProduct(D, BXaxis));
	float B1 = fabsf(FVector::DotProduct(D, BYaxis));
	float B2 = fabsf(FVector::DotProduct(D, BZaxis));

	//L R0 R1 R
	float R0, R1, R;

	//1 (Ra)x
	R0	= ABoxExtent.GetX();
	R1	= BBoxExtent.GetX() * fabsf(XAxis.GetX()) + BBoxExtent.GetY() * fabsf(XAxis.GetY()) + BBoxExtent.GetZ() * fabsf(XAxis.GetZ());
	R = A0;
	if (R > R0 + R1) { printf("(Ra)x\n"); return false; }

	//2 (Ra)y
	R0 = ABoxExtent.GetY();
	R1 = BBoxExtent.GetX() * fabsf(YAxis.GetX()) + BBoxExtent.GetY() * fabsf(YAxis.GetY()) + BBoxExtent.GetZ() * fabsf(YAxis.GetZ());
	R = A1;
	if (R > R0 + R1) { printf("(Ra)y\n"); return false; }

	//3 (Ra)z
	R0 = ABoxExtent.GetY();
	R1 = BBoxExtent.GetX() * fabsf(ZAxis.GetX()) + BBoxExtent.GetY() * fabsf(ZAxis.GetY()) + BBoxExtent.GetZ() * fabsf(ZAxis.GetZ());
	R = A2;
	if (R > R0 + R1) { printf("(Ra)z\n"); return false; }

	//4 (Rb)x
	R0 = ABoxExtent.GetX() * fabsf(XAxis.GetX()) + ABoxExtent.GetY() * fabsf(YAxis.GetX()) + ABoxExtent.GetZ() * fabsf(ZAxis.GetX());
	R1 = BBoxExtent.GetX();
	R = B0;
	if (R > R0 + R1) { printf("(Rb)x\n"); return false; }

	//5 (Rb)y
	R0 = ABoxExtent.GetX() * fabsf(XAxis.GetY()) + ABoxExtent.GetY() * fabsf(YAxis.GetY()) + ABoxExtent.GetZ() * fabsf(ZAxis.GetY());
	R1 = BBoxExtent.GetY();
	R = B1;
	if (R > R0 + R1) { printf("(Rb)y\n"); return false; }

	//6 (Rb)z
	R0 = ABoxExtent.GetX() * fabsf(XAxis.GetZ()) + ABoxExtent.GetY() * fabsf(YAxis.GetZ()) + ABoxExtent.GetZ() * fabsf(ZAxis.GetZ());
	R1 = BBoxExtent.GetZ();
	R = B2;
	if (R > R0 + R1) { printf("(Rb)z\n"); return false; }

	//7 (Ra)x X (Rb)x
	R0 = ABoxExtent.GetY() * fabsf(ZAxis.GetX()) + ABoxExtent.GetZ() * fabsf(YAxis.GetX());
	R1 = BBoxExtent.GetY() * fabsf(XAxis.GetZ()) + BBoxExtent.GetZ() * fabsf(XAxis.GetY());
	R = (A2 * YAxis.GetX() - A1 * ZAxis.GetX());
	if (R > R0 + R1) { printf("(Ra)x X (Rb)x\n"); return false; }

	//8 (Ra)x X (Rb)y
	R0 = ABoxExtent.GetY() * fabsf(ZAxis.GetY()) + ABoxExtent.GetZ() * fabsf(YAxis.GetZ());
	R1 = BBoxExtent.GetX() * fabsf(XAxis.GetZ()) + BBoxExtent.GetZ() * fabsf(XAxis.GetX());
	R = (A2 * YAxis.GetY() - A1 * ZAxis.GetY());
	if (R > R0 + R1) { printf("(Ra)x X (Rb)y\n"); return false; }

	//9 (Ra)x X (Rb)z
	R0 = ABoxExtent.GetY() * fabsf(ZAxis.GetZ()) + ABoxExtent.GetZ() * fabsf(XAxis.GetX());
	R1 = BBoxExtent.GetX() * fabsf(XAxis.GetY()) + BBoxExtent.GetY() * fabsf(XAxis.GetX());
	R = (A2 * YAxis.GetZ() - A1 * ZAxis.GetZ());
	if (R > R0 + R1) { printf("(Ra)x X (Rb)z\n"); return false; }

	//10 (Ra)y X (Rb)x
	R0 = ABoxExtent.GetX() * fabsf(ZAxis.GetX()) + ABoxExtent.GetZ() * fabsf(XAxis.GetX());
	R1 = BBoxExtent.GetY() * fabsf(YAxis.GetZ()) + BBoxExtent.GetZ() * fabsf(YAxis.GetY());
	R = (A0 * ZAxis.GetX() - A2 * XAxis.GetX());
	if (R > R0 + R1) { printf("(Ra)y X (Rb)x\n"); return false; }

	//11 (Ra)y X (Rb)y
	R0 = ABoxExtent.GetX() * fabsf(ZAxis.GetY()) + ABoxExtent.GetZ() * fabsf(XAxis.GetY());
	R1 = BBoxExtent.GetX() * fabsf(XAxis.GetZ()) + BBoxExtent.GetZ() * fabsf(YAxis.GetX());
	R = (A0 * ZAxis.GetY() - A2 * XAxis.GetY());
	if (R > R0 + R1) { printf("(Ra)y X (Rb)y\n"); return false; }

	//12 (Ra)y X (Rb)z
	R0 = ABoxExtent.GetX() * fabsf(ZAxis.GetZ()) + ABoxExtent.GetZ() * fabsf(XAxis.GetZ());
	R1 = BBoxExtent.GetX() * fabsf(YAxis.GetY()) + BBoxExtent.GetY() * fabsf(YAxis.GetX());
	R = (A0 * ZAxis.GetZ() - A2 * XAxis.GetZ());
	if (R > R0 + R1) { printf("(Ra)y X (Rb)z\n"); return false; }

	//13 (Ra)z X (Rb)x
	R0 = ABoxExtent.GetX() * fabsf(YAxis.GetX()) + ABoxExtent.GetY() * fabsf(XAxis.GetX());
	R1 = BBoxExtent.GetY() * fabsf(ZAxis.GetZ()) + BBoxExtent.GetZ() * fabsf(ZAxis.GetY());
	R = (A1 * XAxis.GetX() - A0 * YAxis.GetX());
	if (R > R0 + R1) { printf("(Ra)z X (Rb)x\n"); return false; }

	//14 (Ra)z X (Rb)y
	R0 = ABoxExtent.GetZ() * fabsf(YAxis.GetY()) + ABoxExtent.GetY() * fabsf(XAxis.GetY());
	R1 = BBoxExtent.GetX() * fabsf(ZAxis.GetZ()) + BBoxExtent.GetZ() * fabsf(ZAxis.GetX());
	R = (A1 * XAxis.GetY() - A0 * YAxis.GetY());
	if (R > R0 + R1) { printf("(Ra)z X (Rb)y\n"); return false; }

	//15 (Ra)z X (Rb)z
	R0 = ABoxExtent.GetX() * fabsf(YAxis.GetZ()) + ABoxExtent.GetY() * fabsf(XAxis.GetZ());
	R1 = BBoxExtent.GetX() * fabsf(ZAxis.GetY()) + BBoxExtent.GetY() * fabsf(ZAxis.GetX());
	R = (A1 * XAxis.GetZ() - A0 * YAxis.GetZ());
	if (R > R0 + R1) { printf("(Ra)z X (Rb)\n"); return false; }

	return true;
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

bool CapsuleTrace::BoxCollisionTraceAABB(BoxCollisionComponent& inBoxCollisionComponent)
{
	return false;
}

bool CapsuleTrace::BoxCollisionTraceOBB(BoxCollisionComponent& inBoxCollisionComponent)
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

bool SphereTrace::BoxCollisionTraceAABB(BoxCollisionComponent& inBoxCollisionComponent)
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

bool SphereTrace::BoxCollisionTraceOBB(BoxCollisionComponent& inBoxCollisionComponent)
{
	return false;
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

bool FrustumTrace::BoxCollisionTraceAABB(BoxCollisionComponent& inBoxCollisionComponent)
{
	return false;
}

bool FrustumTrace::BoxCollisionTraceOBB(BoxCollisionComponent& inBoxCollisionComponent)
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