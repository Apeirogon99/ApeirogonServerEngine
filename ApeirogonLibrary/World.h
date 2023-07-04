#pragma once

class World : public GameObject
{
public:
	World(const WCHAR* inName, TaskManagerRef inTaskManager);
	virtual ~World();

protected:
	virtual void OnInitialization()					abstract;
	virtual void OnDestroy()						abstract;
	virtual void OnTick(const int64 inDeltaTime)	abstract;
	virtual bool IsValid()							abstract;

public:

	template<typename T>
	ActorPtr		SpawnActor(const Location& inLocation, const Rotation& inRotation, const Scale& inScale);

	template <typename T>
	bool			FindAllActors(std::vector<ActorPtr>& outActors);
	bool			FindActor(const int64 inGameObjectID, ActorPtr& outActor);

	bool			DestroyAllActor();
	bool			DestroyActor(const int64 inGameObjectID);

	bool			IsValidActor(const int64 inGameObjectID);

public:
	WorldPtr		GetWorldPtr()		{ return std::static_pointer_cast<World>(shared_from_this()); }
	WorldRef		GetWorldRef()		{ return std::static_pointer_cast<World>(shared_from_this()); }
	TaskManagerRef	GetTaksManagerRef() { return mTaskManager; }
	DataManagerPtr	GetDatas()			{ return mTaskManager.lock()->GetService()->GetDataManager(); }
	const int64		GetWorldTime()		{ return mTaskManager.lock()->GetService()->GetServiceTimeStamp(); }

protected:
	TaskManagerRef						mTaskManager;
	std::map<int64, ActorPtr>			mWorldActors;
};
