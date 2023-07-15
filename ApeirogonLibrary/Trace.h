#pragma once

enum class ETraceType
{
	Trace_None,
	Trace_Box,
	Trace_Capsule,
	Trace_Sphere,
	Trace_Frustum,
};

class APEIROGON_API Trace
{
public:
	Trace(FVector inStart, FVector inEnd, bool inIsIgnore, ETraceType inType) : mStart(inStart), mEnd(inEnd), mIsIgnore(inIsIgnore), mTraceType(inType) {}
	~Trace() {}

	Trace(const Trace&) = delete;
	Trace(Trace&&) noexcept = delete;

	Trace& operator=(const Trace&) = delete;
	Trace& operator=(Trace&&) noexcept = delete;

public:
	bool HitCollision(Collision& inCollision);

public:
	const ETraceType& GetTraceType() const;

protected:
	virtual bool BoxCollisionTrace(BoxCollisionComponent& inBoxCollisionComponent)	abstract;
	virtual bool CapsuleCollisionTrace(const CapsuleCollision& inCapsuleCollision)	abstract;
	virtual bool SphereCollisionTrace(const SphereCollision& inSphereCollision)		abstract;
	virtual bool FrustumCollisionTrace(const FrustumCollision& inFrustumCollision)	abstract;

protected:
	FVector		mStart;
	FVector		mEnd;
	bool		mIsIgnore;
	ETraceType	mTraceType;
};

class APEIROGON_API BoxTrace : public Trace
{
public:
	BoxTrace(FVector inStart, FVector inEnd, bool inIsIgnore, FVector inBoxExtent, FRotator inOrientation);
	~BoxTrace() {}

	BoxTrace(const BoxTrace&) = delete;
	BoxTrace(BoxTrace&&) noexcept = delete;

	BoxTrace& operator=(const BoxTrace&) = delete;
	BoxTrace& operator=(BoxTrace&&) noexcept = delete;

public:
	virtual bool BoxCollisionTrace(BoxCollisionComponent& inBoxCollisionComponent)	override;
	virtual bool CapsuleCollisionTrace(const CapsuleCollision& inCapsuleCollision)	override;
	virtual bool SphereCollisionTrace(const SphereCollision& inSphereCollision)		override;
	virtual bool FrustumCollisionTrace(const FrustumCollision& inFrustumCollision)	override;

private:
	BoxCollision mBoxCollision;
};

class APEIROGON_API CapsuleTrace : public Trace
{
public:
	CapsuleTrace(FVector inStart, FVector inEnd, bool inIsIgnore, const float inRadius, const float inHeight) : Trace(inStart, inEnd, inIsIgnore, ETraceType::Trace_Capsule), mCapsuleCollision(inRadius, inHeight) {}
	~CapsuleTrace() {}

	CapsuleTrace(const CapsuleTrace&) = delete;
	CapsuleTrace(CapsuleTrace&&) noexcept = delete;

	CapsuleTrace& operator=(const CapsuleTrace&) = delete;
	CapsuleTrace& operator=(CapsuleTrace&&) noexcept = delete;

public:
	virtual bool BoxCollisionTrace(BoxCollisionComponent& inBoxCollisionComponent)	override {return false;}
	virtual bool CapsuleCollisionTrace(const CapsuleCollision& inCapsuleCollision)	override {return false;}
	virtual bool SphereCollisionTrace(const SphereCollision& inSphereCollision)		override {return false;}
	virtual bool FrustumCollisionTrace(const FrustumCollision& inFrustumCollision)	override {return false;}

private:
	CapsuleCollision mCapsuleCollision;
};

class APEIROGON_API SphereTrace : public Trace
{
public:
	SphereTrace(FVector inStart, FVector inEnd, bool inIsIgnore, float inRadius) : Trace(inStart, inEnd, inIsIgnore, ETraceType::Trace_Sphere), mSphereCollision(inRadius) {}
	~SphereTrace() {}

	SphereTrace(const SphereTrace&) = delete;
	SphereTrace(SphereTrace&&) noexcept = delete;

	SphereTrace& operator=(const SphereTrace&) = delete;
	SphereTrace& operator=(SphereTrace&&) noexcept = delete;

public:
	virtual bool BoxCollisionTrace(BoxCollisionComponent& inBoxCollisionComponentn)	override {return false;}
	virtual bool CapsuleCollisionTrace(const CapsuleCollision& inCapsuleCollision)	override {return false;}
	virtual bool SphereCollisionTrace(const SphereCollision& inSphereCollision)		override {return false;}
	virtual bool FrustumCollisionTrace(const FrustumCollision& inFrustumCollision)	override {return false;}

private:
	SphereCollision mSphereCollision;
};

class APEIROGON_API FrustumTrace : public Trace
{
public:
	FrustumTrace(FVector inStart, FVector inEnd, bool inIsIgnore, float inNearPlane, float inFarPlane, float inHeight) : Trace(inStart, inEnd, inIsIgnore, ETraceType::Trace_Frustum), mFrustumCollision(inNearPlane, inFarPlane, inHeight) {}
	~FrustumTrace() {}

	FrustumTrace(const FrustumTrace&) = delete;
	FrustumTrace(FrustumTrace&&) noexcept = delete;

	FrustumTrace& operator=(const FrustumTrace&) = delete;
	FrustumTrace& operator=(FrustumTrace&&) noexcept = delete;

public:
	virtual bool BoxCollisionTrace(BoxCollisionComponent& inBoxCollisionComponent)	override {return false;}
	virtual bool CapsuleCollisionTrace(const CapsuleCollision& inCapsuleCollision)	override {return false;}
	virtual bool SphereCollisionTrace(const SphereCollision& inSphereCollision)		override {return false;}
	virtual bool FrustumCollisionTrace(const FrustumCollision& inFrustumCollision)	override {return false;}

private:
	FrustumCollision mFrustumCollision;
};