#include "pch.h"
#include "AnimationComponent.h"

AnimationComponent::AnimationComponent() : mAnimationType(0), mAnimationCurrentPlayTime(0)
{
}

AnimationComponent::~AnimationComponent()
{
}

void AnimationComponent::SetNewAnimationType(const int32 inNewAnimationType)
{
	if (inNewAnimationType == mAnimationType)
	{
		return;
	}

	mAnimationType = inNewAnimationType;
	mAnimationCurrentPlayTime = 0;
}

bool AnimationComponent::Update(const int64 inUpdateTime)
{

	if (NONE_ANIMATION == mAnimationType)
	{
		return false;
	}

	mAnimationCurrentPlayTime += inUpdateTime;
	return true;
}

const float AnimationComponent::GetCurrentAnimationTime() const
{
	return static_cast<float>(mAnimationCurrentPlayTime / 1000.0f);
}
