#pragma once

enum class ETraceType
{
	Trace_None,
	Trace_Line,
	Trace_Box,
	Trace_Capsule,
	Trace_Sphere,
	Trace_Frustum,
};

class Trace
{
public:
	APEIROGON_API Trace(ActorRef inOwner, FVector inStart, FVector inEnd, bool inIsIgnore, ETraceType inType);
	APEIROGON_API virtual ~Trace() {}

	Trace(const Trace&) = delete;
	Trace(Trace&&) noexcept = delete;

	Trace& operator=(const Trace&) = delete;
	Trace& operator=(Trace&&) noexcept = delete;

public:
	APEIROGON_API const ActorRef&		GetOwner() const;
	APEIROGON_API const ETraceType&	GetTraceType() const; 

public:
	APEIROGON_API virtual bool CollisionTrace(CollisionComponent* inCollision)									abstract;
	APEIROGON_API virtual bool BoxCollisionTraceAABB(BoxCollisionComponent* inBoxCollisionComponent)			abstract;
	APEIROGON_API virtual bool BoxCollisionTraceOBB(BoxCollisionComponent* inBoxCollisionComponent)				abstract;
	APEIROGON_API virtual bool CapsuleCollisionTrace(CapsuleCollisionComponent* inCapsuleCollisionComponent)	abstract;
	APEIROGON_API virtual bool SphereCollisionTrace(SphereCollisionComponent* inSphereCollisionComponent)		abstract;

public:
	APEIROGON_API FVector GetStartLocation() const;
	APEIROGON_API FVector GetEndLocation() const;

protected:
	ActorRef	mOwner;
	FVector		mStart;
	FVector		mEnd;
	bool		mIsIgnore;
	ETraceType	mTraceType;
};

class LineTrace : public Trace
{
public:
	APEIROGON_API LineTrace(ActorRef inOwner, FVector inStart, FVector inEnd, bool inIsIgnore);
	APEIROGON_API virtual ~LineTrace() {}

	LineTrace(const LineTrace&) = delete;
	LineTrace(LineTrace&&) noexcept = delete;

	LineTrace& operator=(const LineTrace&) = delete;
	LineTrace& operator=(LineTrace&&) noexcept = delete;

public:
	APEIROGON_API virtual bool CollisionTrace(CollisionComponent* inCollision)									override;
	APEIROGON_API virtual bool BoxCollisionTraceAABB(BoxCollisionComponent* inBoxCollisionComponent)			override;
	APEIROGON_API virtual bool BoxCollisionTraceOBB(BoxCollisionComponent* inBoxCollisionComponent)				override;
	APEIROGON_API virtual bool CapsuleCollisionTrace(CapsuleCollisionComponent* inCapsuleCollisionComponent)	override;
	APEIROGON_API virtual bool SphereCollisionTrace(SphereCollisionComponent* inSphereCollisionComponent)		override;

public:
	APEIROGON_API FVector GetCenterLocation() const;
	APEIROGON_API float GetDistance() const;
	APEIROGON_API FVector GetImpactPoint() const;

public:
	FVector mImpactPoint;
};

class BoxTrace : public Trace
{
public:
	APEIROGON_API BoxTrace(ActorRef inOwner, FVector inStart, FVector inEnd, bool inIsIgnore, FVector inBoxExtent, FRotator inOrientation);
	APEIROGON_API virtual ~BoxTrace() {}

	BoxTrace(const BoxTrace&) = delete;
	BoxTrace(BoxTrace&&) noexcept = delete;

	BoxTrace& operator=(const BoxTrace&) = delete;
	BoxTrace& operator=(BoxTrace&&) noexcept = delete;

public:
	APEIROGON_API virtual bool InitBoxTrace(ActorRef inOwner, FVector inStart, FVector inEnd, bool inIsIgnore, FVector inBoxExtent, FRotator inOrientation);
	APEIROGON_API virtual bool CollisionTrace(CollisionComponent* inCollision)									override;
	APEIROGON_API virtual bool BoxCollisionTraceAABB(BoxCollisionComponent* inBoxCollisionComponent)			override;
	APEIROGON_API virtual bool BoxCollisionTraceOBB(BoxCollisionComponent* inBoxCollisionComponent)				override;
	APEIROGON_API virtual bool CapsuleCollisionTrace(CapsuleCollisionComponent* inCapsuleCollisionComponent)	override;
	APEIROGON_API virtual bool SphereCollisionTrace(SphereCollisionComponent* inSphereCollisionComponent)		override;

public:
	APEIROGON_API FVector GetCenterLocation() const;
	APEIROGON_API const BoxCollision& GetBoxCollision() const;

private:
	BoxCollision mBoxCollision;
};

class CapsuleTrace : public Trace
{
public:
	APEIROGON_API CapsuleTrace(ActorRef inOwner, FVector inStart, FVector inEnd, bool inIsIgnore, const float inRadius, const float inHeight); 
	APEIROGON_API virtual ~CapsuleTrace();

	CapsuleTrace(const CapsuleTrace&) = delete;
	CapsuleTrace(CapsuleTrace&&) noexcept = delete;

	CapsuleTrace& operator=(const CapsuleTrace&) = delete;
	CapsuleTrace& operator=(CapsuleTrace&&) noexcept = delete;

public:
	APEIROGON_API virtual bool CollisionTrace(CollisionComponent* inCollision)									override;
	APEIROGON_API virtual bool BoxCollisionTraceAABB(BoxCollisionComponent* inBoxCollisionComponent)			override;
	APEIROGON_API virtual bool BoxCollisionTraceOBB(BoxCollisionComponent* inBoxCollisionComponent)				override;
	APEIROGON_API virtual bool CapsuleCollisionTrace(CapsuleCollisionComponent* inCapsuleCollisionComponent)	override;
	APEIROGON_API virtual bool SphereCollisionTrace(SphereCollisionComponent* inSphereCollisionComponent)		override;

public:
	APEIROGON_API FVector GetCenterLocation() const;
	APEIROGON_API const CapsuleCollision& GetCapsuleCollision() const;

private:
	CapsuleCollision mCapsuleCollision;
};

class SphereTrace : public Trace
{
public:
	APEIROGON_API SphereTrace(ActorRef inOwner, FVector inLocation, bool inIsIgnore, float inRadius);
	APEIROGON_API virtual ~SphereTrace();

	SphereTrace(const SphereTrace&) = delete;
	SphereTrace(SphereTrace&&) noexcept = delete;

	SphereTrace& operator=(const SphereTrace&) = delete;
	SphereTrace& operator=(SphereTrace&&) noexcept = delete;

public:
	APEIROGON_API virtual bool CollisionTrace(CollisionComponent* inCollision)									override;
	APEIROGON_API virtual bool BoxCollisionTraceAABB(BoxCollisionComponent* inBoxCollisionComponent)			override;
	APEIROGON_API virtual bool BoxCollisionTraceOBB(BoxCollisionComponent* inBoxCollisionComponent)				override;
	APEIROGON_API virtual bool CapsuleCollisionTrace(CapsuleCollisionComponent* inCapsuleCollisionComponent)	override;
	APEIROGON_API virtual bool SphereCollisionTrace(SphereCollisionComponent* inSphereCollisionComponent)		override;

public:
	APEIROGON_API FVector GetCenterLocation() const;
	APEIROGON_API const SphereCollision& GetSphereCollision() const;

private:
	SphereCollision mSphereCollision;
};

class FrustumTrace : public Trace
{
public:
	APEIROGON_API FrustumTrace(ActorRef inOwner, FVector inStart, FVector inEnd, bool inIsIgnore, float inNearPlane, float inFarPlane, float inHeight);
	APEIROGON_API virtual ~FrustumTrace();

	FrustumTrace(const FrustumTrace&) = delete;
	FrustumTrace(FrustumTrace&&) noexcept = delete;

	FrustumTrace& operator=(const FrustumTrace&) = delete;
	FrustumTrace& operator=(FrustumTrace&&) noexcept = delete;

public:
	APEIROGON_API virtual bool CollisionTrace(CollisionComponent* inCollision)									override;
	APEIROGON_API virtual bool BoxCollisionTraceAABB(BoxCollisionComponent* inBoxCollisionComponent)			override;
	APEIROGON_API virtual bool BoxCollisionTraceOBB(BoxCollisionComponent* inBoxCollisionComponent)				override;
	APEIROGON_API virtual bool CapsuleCollisionTrace(CapsuleCollisionComponent* inCapsuleCollisionComponent)	override;
	APEIROGON_API virtual bool SphereCollisionTrace(SphereCollisionComponent* inSphereCollisionComponent)		override;

private:
	FrustumCollision mFrustumCollision;
};