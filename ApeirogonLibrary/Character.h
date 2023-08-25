#pragma once
class Character : public Actor
{
public:
	APEIROGON_API Character(const WCHAR* inActorName);
	APEIROGON_API virtual ~Character();

public:
	APEIROGON_API virtual void OnInitialization()							abstract;
	APEIROGON_API virtual void OnDestroy()									abstract;
	APEIROGON_API virtual void OnTick(const int64 inDeltaTime)				abstract;
	APEIROGON_API virtual bool IsValid()									abstract;

public:
	APEIROGON_API virtual void OnAppearActor(ActorPtr inAppearActor)		abstract;
	APEIROGON_API virtual void OnDisAppearActor(ActorPtr inDisAppearActor)	abstract;

public:



public:
	APEIROGON_API MovementComponent&		GetMovementComponent();
	APEIROGON_API AnimationComponent&		GetAnimationComponent();
	APEIROGON_API BoxCollisionComponent*	GetCapsuleCollisionComponent();

protected:
	MovementComponent					mMovementComponent;
	AnimationComponent					mAnimationComponent;
};

