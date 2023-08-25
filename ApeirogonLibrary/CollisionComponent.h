#pragma once
class CollisionComponent
{
public:
	APEIROGON_API CollisionComponent() : mCollisionType(CollisionType::Collision_Unspecified) {}
	APEIROGON_API ~CollisionComponent() {}

	CollisionComponent(const CollisionComponent&) = delete;
	CollisionComponent(CollisionComponent&&) noexcept = delete;

	CollisionComponent& operator=(const CollisionComponent&) = delete;
	CollisionComponent& operator=(CollisionComponent&&) noexcept = delete;

public:
	APEIROGON_API void InitCollisionComponent(ActorRef inOwner, const FVector& inLocalLocation, const FRotator& inLocalRotation);
	APEIROGON_API void SetOwner(ActorRef inOwner);
	APEIROGON_API void SetLocalLocation(const FVector& inLocalLocation);
	APEIROGON_API void SetLocalRotation(const FRotator& inLocalRotation);
	APEIROGON_API void SetCollisionType(const CollisionType& inColisionType);

public:
	APEIROGON_API const ActorRef&		GetOwner() const;
	APEIROGON_API virtual const float	GetLocalRadius() abstract;
	APEIROGON_API const FVector&		GetLocalLocation() const;
	APEIROGON_API const FRotator&		GetLocalRotation() const;
	APEIROGON_API const CollisionType&	GetCollisionType() const;

protected:
	ActorRef		mOwner;
	float			mLocalRadius;
	FVector			mLocalLocation;
	FRotator		mLocalRotation;
	CollisionType	mCollisionType;
};

class BoxCollisionComponent : public CollisionComponent
{
public:
	APEIROGON_API BoxCollisionComponent();
	APEIROGON_API ~BoxCollisionComponent();

	BoxCollisionComponent(const BoxCollisionComponent&) = delete;
	BoxCollisionComponent(BoxCollisionComponent&&) noexcept = delete;

	BoxCollisionComponent& operator=(const BoxCollisionComponent&) = delete;
	BoxCollisionComponent& operator=(BoxCollisionComponent&&) noexcept = delete;

public:
	APEIROGON_API void SetBoxCollision(const FVector& inBoxExtent);

public:
	APEIROGON_API virtual const float GetLocalRadius() override;
	APEIROGON_API BoxCollision& GetBoxCollision();

private:
	BoxCollision mBoxCollision;
};

class CapsuleCollisionComponent : public CollisionComponent
{
public:
	APEIROGON_API CapsuleCollisionComponent();
	APEIROGON_API ~CapsuleCollisionComponent();

	CapsuleCollisionComponent(const CapsuleCollisionComponent&) = delete;
	CapsuleCollisionComponent(CapsuleCollisionComponent&&) noexcept = delete;

	CapsuleCollisionComponent& operator=(const CapsuleCollisionComponent&) = delete;
	CapsuleCollisionComponent& operator=(CapsuleCollisionComponent&&) noexcept = delete;

public:
	APEIROGON_API void SetCapsuleCollision(const float inRadius, const float inHeight);

public:
	APEIROGON_API virtual const float GetLocalRadius() override;
	APEIROGON_API const CapsuleCollision& GetCapsuleCollision();

private:
	CapsuleCollision mCapsuleCollision;
};

class SphereCollisionComponent : public CollisionComponent
{
public:
	APEIROGON_API SphereCollisionComponent();
	APEIROGON_API ~SphereCollisionComponent();

	SphereCollisionComponent(const SphereCollisionComponent&) = delete;
	SphereCollisionComponent(SphereCollisionComponent&&) noexcept = delete;

	SphereCollisionComponent& operator=(const SphereCollisionComponent&) = delete;
	SphereCollisionComponent& operator=(SphereCollisionComponent&&) noexcept = delete;

public:
	APEIROGON_API void SetSphereCollisione(const float inRadius);

public:
	APEIROGON_API virtual const float GetLocalRadius() override;
	APEIROGON_API const SphereCollision& GetSphereCollision();

private:
	SphereCollision mSphereCollision;
};