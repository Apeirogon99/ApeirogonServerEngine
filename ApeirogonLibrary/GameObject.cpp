#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(const WCHAR* inObjectName) : mGameObjectName(inObjectName), mGameObjectID(INVALID_GAMEOBJECT_ID), mIsTick(false), mTickTimer(0), mMaxTickTimer(0)
{
}

GameObject::~GameObject()
{
}

void GameObject::Initialization()
{
	SetTick(false, INFINITE);
	OnInitialization();
}

void GameObject::Destroy()
{
	mGameObjectName = nullptr;
	mGameObjectID = INVALID_GAMEOBJECT_ID;
	mTaskManagerRef.reset();
	mIsTick = false;
	OnDestroy();
}

void GameObject::Tick(const int64 inDeltaTime)
{
	if (false == mIsTick)
	{
		return;
	}

	mTickTimer -= inDeltaTime;
	if (mTickTimer < 0)
	{
		OnTick(mMaxTickTimer - mTickTimer);
		mTickTimer = mMaxTickTimer;
	}
}

void GameObject::SetTick(bool inUse, const int64 inMaxTimer)
{
	mIsTick = inUse;
	mMaxTickTimer = inMaxTimer;
	mTickTimer = inMaxTimer;
}

void GameObject::SetTaskManagerRef(TaskManagerRef inTaskManagerRef)
{
	mTaskManagerRef = inTaskManagerRef;
}

void GameObject::SetGameObjectName(const WCHAR* inObjectName)
{
	mGameObjectName = inObjectName;
}

void GameObject::SetGameObjectID(const int64 inObjectID)
{
	mGameObjectID = inObjectID;
}

TaskManagerRef GameObject::GetTaskManagerRef()
{
	return mTaskManagerRef;
}

GameObjectPtr GameObject::GetGameObjectPtr()
{
	return std::static_pointer_cast<GameObject>(shared_from_this());
}

GameObjectRef GameObject::GetGameObjectRef()
{
	return std::static_pointer_cast<GameObject>(shared_from_this());
}

const WCHAR* GameObject::GetGameObjectName() const
{
	return mGameObjectName;
}

const int64 GameObject::GetGameObjectID() const
{
	return mGameObjectID;
}

const int64 GameObject::GetMaxTick() const
{
	return mMaxTickTimer;
}

const int64 GameObject::GetCurrentTick() const
{
	return mTickTimer;
}
