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
	bool TraceCheack(const Collision& inCollision, Hit& outHit);
	bool TraceCheack(const std::map<int64, Collision>& inCollisions, std::vector<Hit>& outHits);

public:
	const ETraceType& GetTraceType() const;

protected:
	virtual bool HitCheack(const Collision& inCollision, Hit& outHit) abstract;

protected:
	FVector		mStart;
	FVector		mEnd;
	bool		mIsIgnore;
	ETraceType	mTraceType;
};

class APEIROGON_API BoxTrace : public Trace
{
public:
	BoxTrace(FVector inStart, FVector inEnd, bool inIsIgnore, FVector inBoxExtent, FRotator inOrientation) : Trace(inStart, inEnd, inIsIgnore, ETraceType::Trace_Box), mExtent(inBoxExtent), mOrientation(inOrientation) {}
	~BoxTrace() {}

	BoxTrace(const BoxTrace&) = delete;
	BoxTrace(BoxTrace&&) noexcept = delete;

	BoxTrace& operator=(const BoxTrace&) = delete;
	BoxTrace& operator=(BoxTrace&&) noexcept = delete;

protected:
	virtual bool HitCheack(const Collision& inCollision, Hit& outHit) override;
	Hit BoxCheack(const BoxCollision& inCollision);

private:
	FVector		mExtent;
	FRotator	mOrientation;
};

class APEIROGON_API CapsuleTrace : public Trace
{
public:
	CapsuleTrace(FVector inStart, FVector inEnd, bool inIsIgnore, const float inRadius, const float inHeight) : Trace(inStart, inEnd, inIsIgnore, ETraceType::Trace_Capsule), mRadius(inRadius), mHeight(inHeight) {}
	~CapsuleTrace() {}

	CapsuleTrace(const CapsuleTrace&) = delete;
	CapsuleTrace(CapsuleTrace&&) noexcept = delete;

	CapsuleTrace& operator=(const CapsuleTrace&) = delete;
	CapsuleTrace& operator=(CapsuleTrace&&) noexcept = delete;

protected:
	virtual bool HitCheack(const Collision& inCollision, Hit& outHit) override;

private:
	float mRadius;
	float mHeight;
};

class APEIROGON_API SphereTrace : public Trace
{
public:
	SphereTrace(FVector inStart, FVector inEnd, bool inIsIgnore, float inRadius) : Trace(inStart, inEnd, inIsIgnore, ETraceType::Trace_Sphere), mRadius(inRadius) {}
	~SphereTrace() {}

	SphereTrace(const SphereTrace&) = delete;
	SphereTrace(SphereTrace&&) noexcept = delete;

	SphereTrace& operator=(const SphereTrace&) = delete;
	SphereTrace& operator=(SphereTrace&&) noexcept = delete;

protected:
	virtual bool HitCheack(const Collision& inCollision, Hit& outHit) override;

private:
	float mRadius;
};

class APEIROGON_API FrustumTrace : public Trace
{
public:
	FrustumTrace(FVector inStart, FVector inEnd, bool inIsIgnore, float inNearPlane, float inFarPlane, float inHeight) : Trace(inStart, inEnd, inIsIgnore, ETraceType::Trace_Frustum), mNearPlane(inNearPlane), mFarPlane(inFarPlane), mHeight(inHeight) {}
	~FrustumTrace() {}

	FrustumTrace(const FrustumTrace&) = delete;
	FrustumTrace(FrustumTrace&&) noexcept = delete;

	FrustumTrace& operator=(const FrustumTrace&) = delete;
	FrustumTrace& operator=(FrustumTrace&&) noexcept = delete;

protected:
	virtual bool HitCheack(const Collision& inCollision, Hit& outHit) override;

private:
	float mNearPlane;
	float mFarPlane;
	float mHeight;
};