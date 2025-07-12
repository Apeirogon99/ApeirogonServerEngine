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
	int64 ProcessTask(const int64 inServiceTimeStamp);
	int64 Tick(const int64 inTickTime);

	//void AllWakeTickThread(const int64 inTickTime);
	//void DoWorkTickThread(const int64 inTickTime, const size_t inMaxSize);

public:
	//APEIROGON_API void		   	CreateGameObject(GameObjectPtr inGameObject);
	//APEIROGON_API void		   	DestroyGameObject(GameObjectPtr inGameObject);

	//APEIROGON_API void		   	PushTask(GameObjectPtr inGameObject);
	//APEIROGON_API void		   	ReleaseTask(GameObjectPtr inGameObject);
	//APEIROGON_API bool			FindTask(const int64 inGameObjectID, GameObjectPtr& outGameObject);
	//APEIROGON_API bool			FindTask(const WCHAR* inGameObjectName, GameObjectPtr& outGameObject);

public:
	APEIROGON_API ServicePtr	GetService() { return mService; }

protected:
	//const int64 NextGameObjectNumber();

public:
	APEIROGON_API void		TaskManagerLog(const WCHAR* log, ...);

private:
	ServicePtr									mService;
	int64										mGameObjectIDCount;
	int64										mCurrentTickGameObjectCount;
	int64										mCurrentGameObjectCount;
	//std::unordered_map<int64, GameObjectPtr>	mGameObjects;
	std::vector<WorldPtr>						mWorlds;


	FastSpinLock								mSpinLock;
	size_t										mObjectCount;

	TimeStamp									mTaskProcessTimeStamp;
	TimeStamp									mTickProcessTimeStamp;
};

