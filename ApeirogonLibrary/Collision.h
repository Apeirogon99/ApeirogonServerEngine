#pragma once

enum class CollisionType : uint8
{
	Collision_Unspecified,
	Collision_Box,
	Collision_Capsule,
	Collision_Sphere,
	Collision_Frustum,
};

class Collision;
class BoxCollision;
class CapsuleCollision;
class SphereCollision;
class FrustumCollision;

class APEIROGON_API Collision
{
public:
	Collision(const CollisionType inCollsionType) : mCollisionType(inCollsionType) {}
	virtual ~Collision() {}

	Collision(const Collision&) = delete;
	Collision(Collision&&) noexcept = delete;

	Collision& operator=(const Collision&) = delete;
	Collision& operator=(Collision&&) noexcept = delete;

public:
	const CollisionType& GetCollisionType() const { return mCollisionType; }

public:
	virtual bool BoxCollisionCheck(const BoxCollision& inBoxCollision)				abstract;
	virtual bool CapsuleCollisionCheck(const CapsuleCollision& inCapsuleCollision)	abstract;
	virtual bool SphereCollisionCheck(const SphereCollision& inSphereCollision)		abstract;
	virtual bool FrustumCollisionCheck(const FrustumCollision& inFrustumCollision)	abstract;

protected:
	CollisionType mCollisionType;
};

class APEIROGON_API BoxCollision : public Collision
{
public:
	BoxCollision(const FVector& inBoxExtent, const FRotator& inOrientation);
	virtual ~BoxCollision();

	BoxCollision(const BoxCollision&) = delete;
	BoxCollision(BoxCollision&&) noexcept = delete;

	BoxCollision& operator=(const BoxCollision&) = delete;
	BoxCollision& operator=(BoxCollision&&) noexcept = delete;

public:
	void SetBoxExtent(const FVector& inBoxExtent) { mExtent = inBoxExtent; }
	void SetOrientation(const FRotator& inOrientation) { mOrientation = inOrientation; }
	void MakeAABB(const FVector& inCenterLocation, FVector& outMin, FVector& outMax);
	void MakeOBB(const FVector& inCenterLocation, Matrix& outMatrix, Matrix& outInvMatrix);

public:
	const FVector&	GetBoxExtent() const { return mExtent; }
	const FRotator& GetOrientation() const { return mOrientation; }

public:
	virtual bool BoxCollisionCheck(const BoxCollision& inBoxCollision)				override;
	virtual bool CapsuleCollisionCheck(const CapsuleCollision& inCapsuleCollision)	override;
	virtual bool SphereCollisionCheck(const SphereCollision& inSphereCollision)		override;
	virtual bool FrustumCollisionCheck(const FrustumCollision& inFrustumCollision)	override;

private:
	FVector		mExtent;
	FRotator	mOrientation;
};

class APEIROGON_API CapsuleCollision : public Collision
{
public:
	CapsuleCollision(const float inRadius, const float inHeight);
	virtual ~CapsuleCollision();

	CapsuleCollision(const CapsuleCollision&) = delete;
	CapsuleCollision(CapsuleCollision&&) noexcept = delete;

	CapsuleCollision& operator=(const CapsuleCollision&) = delete;
	CapsuleCollision& operator=(CapsuleCollision&&) noexcept = delete;

public:
	void SetRadius(const float inRadius) { mRadius = inRadius; }
	void SetHeight(const float inHeight) { mHeight = inHeight; }

public:
	const float GetRadius() const { return mRadius; }
	const float GetHeight() const { return mHeight; }

public:
	virtual bool BoxCollisionCheck(const BoxCollision& inBoxCollision)				override;
	virtual bool CapsuleCollisionCheck(const CapsuleCollision& inCapsuleCollision)	override;
	virtual bool SphereCollisionCheck(const SphereCollision& inSphereCollision)		override;
	virtual bool FrustumCollisionCheck(const FrustumCollision& inFrustumCollision)	override;

private:
	float mRadius;
	float mHeight;
};

class APEIROGON_API SphereCollision : public Collision
{
public:
	SphereCollision(const float inRadius);
	virtual ~SphereCollision();

	SphereCollision(const SphereCollision&) = delete;
	SphereCollision(SphereCollision&&) noexcept = delete;

	SphereCollision& operator=(const SphereCollision&) = delete;
	SphereCollision& operator=(SphereCollision&&) noexcept = delete;

public:
	void SetRadius(const float inRadius) { mRadius = inRadius; }

public:
	const float GetRadius() const { return mRadius; }

public:
	virtual bool BoxCollisionCheck(const BoxCollision& inBoxCollision)				override;
	virtual bool CapsuleCollisionCheck(const CapsuleCollision& inCapsuleCollision)	override;
	virtual bool SphereCollisionCheck(const SphereCollision& inSphereCollision)		override;
	virtual bool FrustumCollisionCheck(const FrustumCollision& inFrustumCollision)	override;

private:
	float mRadius;

};

class APEIROGON_API FrustumCollision : public Collision
{
public:
	FrustumCollision(const float inNearPlane, const float inFarPlane, const float inHeight);
	virtual ~FrustumCollision();

	FrustumCollision(const FrustumCollision&) = delete;
	FrustumCollision(FrustumCollision&&) noexcept = delete;

	FrustumCollision& operator=(const FrustumCollision&) = delete;
	FrustumCollision& operator=(FrustumCollision&&) noexcept = delete;

public:
	void SetNearPlane(const float inNearPlane) { mNearPlane = inNearPlane; }
	void SetFarPlane(const float inFarPlane) { mFarPlane = inFarPlane; }
	void SetHeight(const float inHeight) { mHeight = inHeight; }

public:
	const float GetNearPlane()	const { return mNearPlane; }
	const float GetFarPlane()	const { return mFarPlane; }
	const float GetHeight()		const { return mHeight; }

public:
	virtual bool BoxCollisionCheck(const BoxCollision& inBoxCollision)				override;
	virtual bool CapsuleCollisionCheck(const CapsuleCollision& inCapsuleCollision)	override;
	virtual bool SphereCollisionCheck(const SphereCollision& inSphereCollision)		override;
	virtual bool FrustumCollisionCheck(const FrustumCollision& inFrustumCollision)	override;

private:
	float mNearPlane;
	float mFarPlane;
	float mHeight;
};