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
	APEIROGON_API void				SetObjectName(const WCHAR* inObjectName);

public:
	APEIROGON_API GameObjectPtr		GetGameObjectPtr();
	APEIROGON_API const WCHAR*		GetObjectName() const;

protected:
	const WCHAR* mObjectName;
};

