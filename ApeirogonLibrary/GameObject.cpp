#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(const WCHAR* inObjectName) : mGameObjectName(inObjectName), mGameObjectID(-1)
{
}

GameObject::~GameObject()
{
	mGameObjectName = nullptr;
}

void GameObject::SetGameObjectName(const WCHAR* inObjectName)
{
	mGameObjectName = inObjectName;
}

void GameObject::SetGameObjectID(const int64 inObjectID)
{
	mGameObjectID = inObjectID;
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
