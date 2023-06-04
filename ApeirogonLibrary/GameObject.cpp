#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(const WCHAR* inObjectName) : mObjectName(inObjectName)
{
}

GameObject::~GameObject()
{
	mObjectName = nullptr;
}
