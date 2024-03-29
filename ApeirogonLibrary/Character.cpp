#include "pch.h"
#include "Character.h"

Character::Character(const WCHAR* inActorName) : Actor(inActorName)
{
	mDefaultCollisionComponent = new BoxCollisionComponent;
}

Character::~Character()
{

}

MovementComponent& Character::GetMovementComponent()
{
	return mMovementComponent;
}

AnimationComponent& Character::GetAnimationComponent()
{
	return mAnimationComponent;
}

BoxCollisionComponent* Character::GetCapsuleCollisionComponent()
{
 	return static_cast<BoxCollisionComponent*>(this->mDefaultCollisionComponent);
}
