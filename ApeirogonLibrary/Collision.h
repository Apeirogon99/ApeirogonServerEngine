#pragma once

enum class CollisionType : uint8
{
	Collision_Unspecified,
	Collision_Box,
	Collision_Capsule,
	Collision_Sphere,
};

class APEIROGON_API Collision
{
public:
	Collision(const CollisionType inCollsionType) : mCollisionType(inCollsionType) {}
	~Collision() {}

	Collision(const Collision&) = delete;
	Collision(Collision&&) noexcept = delete;

	Collision& operator=(const Collision&) = delete;
	Collision& operator=(Collision&&) noexcept = delete;

public:
	const CollisionType& GetCollisionType() const { return mCollisionType; }

protected:
	CollisionType mCollisionType;
};

class APEIROGON_API BoxCollision : public Collision
{
public:
	BoxCollision(const FVector inBoxExtent, const FRotator inOrientation) : Collision(CollisionType::Collision_Box), mExtent(inBoxExtent), mOrientation(inOrientation) {}
	~BoxCollision() {}

	BoxCollision(const BoxCollision&) = delete;
	BoxCollision(BoxCollision&&) noexcept = delete;

	BoxCollision& operator=(const BoxCollision&) = delete;
	BoxCollision& operator=(BoxCollision&&) noexcept = delete;

public:
	void			SetBoxExtent(const FVector& inBoxExtent) { mExtent = inBoxExtent; }
	void			SetOrientation(const FRotator& inOrientation) { mOrientation = inOrientation; }

public:
	const FVector&	GetBoxExtent() const { return mExtent; }
	const FRotator& GetOrientation() const { return mOrientation; }

private:
	FVector		mExtent;
	FRotator	mOrientation;
};

class APEIROGON_API CapsuleCollision : public Collision
{
public:
	CapsuleCollision(const float inRadius, const float inHeight) : Collision(CollisionType::Collision_Capsule), mRadius(inRadius), mHeight(inHeight) {}
	~CapsuleCollision() {}

	CapsuleCollision(const CapsuleCollision&) = delete;
	CapsuleCollision(CapsuleCollision&&) noexcept = delete;

	CapsuleCollision& operator=(const CapsuleCollision&) = delete;
	CapsuleCollision& operator=(CapsuleCollision&&) noexcept = delete;

public:
	void SetRadius(const float inRadius) { mRadius = inRadius; }
	void SetHeight(const float inHeight) { mHeight = inHeight; }

private:
	float mRadius;
	float mHeight;
};

class APEIROGON_API SphereCollision : public Collision
{
public:
	SphereCollision(const float inRadius) : Collision(CollisionType::Collision_Sphere), mRadius(inRadius) {}
	~SphereCollision() {}

	SphereCollision(const SphereCollision&) = delete;
	SphereCollision(SphereCollision&&) noexcept = delete;

	SphereCollision& operator=(const SphereCollision&) = delete;
	SphereCollision& operator=(SphereCollision&&) noexcept = delete;

public:
	void SetRadius(const float inRadius) { mRadius = inRadius; }

private:
	float mRadius;

};