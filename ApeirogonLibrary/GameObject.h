#pragma once

class GameObject : public TaskQueue
{
public:
	APEIROGON_API GameObject(const WCHAR* inObjectName);
	APEIROGON_API virtual ~GameObject();

public:
	APEIROGON_API virtual void		Initialization()	{};
	APEIROGON_API virtual void		Destroy()			{};
	APEIROGON_API virtual void		Tick()				abstract;
	APEIROGON_API virtual bool		IsValid()			abstract;
	//APEIROGON_API virtual void	Serialization()		abstract;

public:
	APEIROGON_API void				SetGameObjectName(const WCHAR* inObjectName);
	APEIROGON_API void				SetGameObjectID(const int64 inObjectNumber);

public:
	APEIROGON_API GameObjectPtr		GetGameObjectPtr();
	APEIROGON_API GameObjectRef		GetGameObjectRef();

	APEIROGON_API const WCHAR*		GetGameObjectName() const;
	APEIROGON_API const int64		GetGameObjectID() const;

protected:
	const WCHAR*	mGameObjectName;
	int64			mGameObjectID;
};

