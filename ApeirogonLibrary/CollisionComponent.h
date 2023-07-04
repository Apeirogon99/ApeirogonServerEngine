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
	void SetOwner(ActorRef mOwner) {}

public:


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
	void SetCapsule(const float inRadius, const float inHeight);

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

private:
	SphereCollision mSphereCollision;
};