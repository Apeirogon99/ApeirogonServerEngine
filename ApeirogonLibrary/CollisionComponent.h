#pragma once
class CollisionComponent
{
public:
	CollisionComponent() {}
	~CollisionComponent() {}

	CollisionComponent(const CollisionComponent&) = delete;
	CollisionComponent(CollisionComponent&&) noexcept = delete;

	CollisionComponent& operator=(const CollisionComponent&) = delete;
	CollisionComponent& operator=(CollisionComponent&&) noexcept = delete;

public:
	void SetOwner(ActorRef inOwner) { mOwner = inOwner; }

public:
	const ActorRef& GetOwner() const { return mOwner; }

protected:
	ActorRef mOwner;
};

class BoxCollisionComponent : public CollisionComponent
{
public:
	BoxCollisionComponent();
	~BoxCollisionComponent();

	BoxCollisionComponent(const BoxCollisionComponent&) = delete;
	BoxCollisionComponent(BoxCollisionComponent&&) noexcept = delete;

	BoxCollisionComponent& operator=(const BoxCollisionComponent&) = delete;
	BoxCollisionComponent& operator=(BoxCollisionComponent&&) noexcept = delete;

public:
	APEIROGON_API void SetBoxCollision(const FVector& inBoxExtent);

public:
	APEIROGON_API BoxCollision& GetBoxCollision();

private:
	BoxCollision mBoxCollision;
};

class CapsuleCollisionComponent : public CollisionComponent
{
public:
	CapsuleCollisionComponent();
	~CapsuleCollisionComponent();

	CapsuleCollisionComponent(const CapsuleCollisionComponent&) = delete;
	CapsuleCollisionComponent(CapsuleCollisionComponent&&) noexcept = delete;

	CapsuleCollisionComponent& operator=(const CapsuleCollisionComponent&) = delete;
	CapsuleCollisionComponent& operator=(CapsuleCollisionComponent&&) noexcept = delete;

public:
	APEIROGON_API void SetCapsuleCollision(const float inRadius, const float inHeight);

public:
	APEIROGON_API const CapsuleCollision& GetCapsuleCollision();

private:
	CapsuleCollision mCapsuleCollision;
};

class SphereCollisionComponent : public CollisionComponent
{
public:
	SphereCollisionComponent();
	~SphereCollisionComponent();

	SphereCollisionComponent(const SphereCollisionComponent&) = delete;
	SphereCollisionComponent(SphereCollisionComponent&&) noexcept = delete;

	SphereCollisionComponent& operator=(const SphereCollisionComponent&) = delete;
	SphereCollisionComponent& operator=(SphereCollisionComponent&&) noexcept = delete;

public:
	APEIROGON_API void SetSphereCollisione(const float inRadius);

public:
	APEIROGON_API const SphereCollision& GetSphereCollision();

private:
	SphereCollision mSphereCollision;
};