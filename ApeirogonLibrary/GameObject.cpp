#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(const WCHAR* inObjectName) : mObjectName(inObjectName)
{
}

GameObject::~GameObject()
{
	mObjectName = nullptr;
}

void GameObject::SetObjectName(const WCHAR* inObjectName)
{
	mObjectName = inObjectName;
}

GameObjectPtr GameObject::GetGameObjectPtr()
{
	return std::static_pointer_cast<GameObject>(shared_from_this());
}

const WCHAR* GameObject::GetObjectName() const
{
	return mObjectName;
}
