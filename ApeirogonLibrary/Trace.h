#pragma once

class Trace
{
public:
	Trace(FVector inStart, FVector inEnd, bool inIsIgnore) : mStart(inStart), mEnd(inEnd), mIsIgnore(inIsIgnore) {}
	~Trace() {}

	Trace(const Trace&) = delete;
	Trace(Trace&&) noexcept = delete;

	Trace& operator=(const Trace&) = delete;
	Trace& operator=(Trace&&) noexcept = delete;

public:
	bool TraceCheack(const Collision& inCollision, Hit& outHit);
	bool TraceCheack(const std::map<int64, Collision>& inCollisions, std::vector<Hit>& outHits);

protected:
	virtual bool HitCheack(const Collision& inCollision, Hit& outHit) abstract;

protected:
	FVector mStart;
	FVector mEnd;
	bool	mIsIgnore;
};

class BoxTrace : public Trace
{
public:
	BoxTrace(FVector inStart, FVector inEnd, bool inIsIgnore, FVector inBoxExtent, FRotator inOrientation) : Trace(inStart, inEnd, inIsIgnore), mExtent(inBoxExtent), mOrientation(inOrientation) {}
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

class CapsuleTrace : public Trace
{
public:
	CapsuleTrace(FVector inStart, FVector inEnd, bool inIsIgnore, const float inRadius, const float inHeight) : Trace(inStart, inEnd, inIsIgnore), mRadius(inRadius), mHeight(inHeight) {}
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

class SphereTrace : public Trace
{
public:
	SphereTrace(FVector inStart, FVector inEnd, bool inIsIgnore, float inRadius) : Trace(inStart, inEnd, inIsIgnore), mRadius(inRadius) {}
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

class FrustumTrace : public Trace
{
public:
	FrustumTrace(FVector inStart, FVector inEnd, bool inIsIgnore, float inNearPlane, float inFarPlane, float inHeight) : Trace(inStart, inEnd, inIsIgnore), mNearPlane(inNearPlane), mFarPlane(inFarPlane), mHeight(inHeight) {}
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