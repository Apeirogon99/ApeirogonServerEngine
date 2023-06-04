#pragma once
class GameObject
{
public:
	APEIROGON_API GameObject(const WCHAR* inObjectName);
	APEIROGON_API virtual ~GameObject();

public:
	APEIROGON_API virtual void Initialization() abstract;
	APEIROGON_API virtual void Destroy()		abstract;
	APEIROGON_API virtual void Tick()			abstract;
	APEIROGON_API virtual bool IsValid()		abstract;

protected:
	const WCHAR* mObjectName;
};

