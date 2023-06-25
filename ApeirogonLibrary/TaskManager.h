#pragma once

class TaskManager : public std::enable_shared_from_this<TaskManager>
{
public:
	APEIROGON_API TaskManager();
	APEIROGON_API virtual ~TaskManager();

private:
	TaskManager(const TaskManager&) = delete;
	TaskManager(TaskManager&&) = delete;

	TaskManager& operator=(const TaskManager&) = delete;
	TaskManager& operator=(TaskManager&&) = delete;

public:
	bool Prepare(ServicePtr service);
	void Shutdown();

public:
	virtual void Init() abstract;
	void ProcessTask(const int64 inServiceTimeStamp);
	void Tick();

public:
	APEIROGON_API void		   	CreateGameObject(GameObjectPtr inGameObject);
	APEIROGON_API void		   	DestroyGameObject(GameObjectPtr inGameObject);

	APEIROGON_API void		   	PushTask(GameObjectPtr inGameObject);
	APEIROGON_API void		   	ReleaseTask(GameObjectPtr inGameObject);
	APEIROGON_API bool			FindTask(const int64 inGameObjectID, GameObjectPtr& outGameObject);
	APEIROGON_API bool			FindTask(const WCHAR* inGameObjectName, GameObjectPtr& outGameObject);

public:
	APEIROGON_API ServicePtr	GetService() { return mService; }

protected:
	const int64 NextGameObjectNumber();
	void		TaskManagerLog(const WCHAR* log, ...);

private:
	ServicePtr									mService;
	int64										mGameObjectCount;
	std::unordered_map<int64, GameObjectPtr>	mGameObjects;
};

