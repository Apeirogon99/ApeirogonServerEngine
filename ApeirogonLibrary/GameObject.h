#pragma once

#define INVALID_GAMEOBJECT_ID 0

class GameObject : public TaskQueue
{
public:
	APEIROGON_API GameObject(const WCHAR* inObjectName);
	APEIROGON_API virtual ~GameObject();

public:
	APEIROGON_API virtual void		Initialization();
	APEIROGON_API virtual void		Destroy();
	APEIROGON_API virtual void		Tick(const int64 inDeltaTime);

	APEIROGON_API virtual void		OnInitialization()				abstract;
	APEIROGON_API virtual void		OnDestroy()						abstract;
	APEIROGON_API virtual void		OnTick(const int64 inDeltaTime)	abstract;
	APEIROGON_API virtual bool		IsValid()						abstract;
	//APEIROGON_API virtual void	Serialization()					abstract;

public:
	APEIROGON_API void				SetOwner(GameObjectRef inOwner);
	APEIROGON_API void				SetTick(bool inUse = false, const int64 inMaxTimer = INFINITE);
	APEIROGON_API void				SetTaskManagerRef(TaskManagerRef inTaskManagerRef);
	APEIROGON_API void				SetGameObjectName(const WCHAR* inObjectName);
	APEIROGON_API void				SetGameObjectID(const int64 inObjectNumber);

public:
	APEIROGON_API TaskManagerRef	GetTaskManagerRef();

	APEIROGON_API GameObjectRef		GetOwner();
	APEIROGON_API GameObjectPtr		GetGameObjectPtr();
	APEIROGON_API GameObjectRef		GetGameObjectRef();

	APEIROGON_API const WCHAR*		GetGameObjectName() const;
	APEIROGON_API const int64		GetGameObjectID()	const;
	APEIROGON_API const int64		GetMaxTick()		const;
	APEIROGON_API const int64		GetCurrentTick()	const;

public:
	template<typename... Args>
	APEIROGON_API void GameObjectLog(const WCHAR* inLog, Args... args);

protected:
	TaskManagerRef	mTaskManagerRef;

	GameObjectRef	mOwner;
	const WCHAR*	mGameObjectName;
	int64			mGameObjectID;

	bool			mIsTick;
	int64			mTickTimer;
	int64			mMaxTickTimer;
};

template<typename ...Args>
inline void GameObject::GameObjectLog(const WCHAR* inLog, Args ...inArgs)
{
	const WCHAR* name	= GetGameObjectName();
	const int64	 id		= GetGameObjectID();

	std::wstringstream tempStream;
	tempStream << L"[" << name << L"::" << id << L"] " << inLog;

	TaskManagerPtr task = mTaskManagerRef.lock();
	if (nullptr == task)
	{
		return;
	}

	task->TaskManagerLog(inLog, inArgs...);
}
