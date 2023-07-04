#pragma once
class APEIROGON_API AnimationComponent
{
public:
	AnimationComponent();
	~AnimationComponent();

	AnimationComponent(const AnimationComponent&) = delete;
	AnimationComponent(AnimationComponent&&) noexcept = delete;

	AnimationComponent& operator=(const AnimationComponent&) = delete;
	AnimationComponent& operator=(AnimationComponent&&) noexcept = delete;

public:
	void		SetNewAnimationType(const int32 inNewAnimationType);
	bool		Update(const int64 inUpdateTime);
	const float	GetCurrentAnimationTime() const;

private:
	int32 mAnimationType;
	int64 mAnimationCurrentPlayTime;
	int64 mAnimationLastTick;
};

