#pragma once

#include "KDTree.h"

class World : public GameObject
{
public:
	APEIROGON_API World(const WCHAR* inName);
	APEIROGON_API virtual ~World();

protected:
	APEIROGON_API virtual void OnInitialization()					abstract;
	APEIROGON_API virtual void OnDestroy()							abstract;
	APEIROGON_API virtual void OnTick(const int64 inDeltaTime)		abstract;
	APEIROGON_API virtual bool IsValid()							abstract;

public:

	template<typename T>
	APEIROGON_API ActorPtr			SpawnActor(GameObjectRef inOwner, const Location& inLocation, const Rotation& inRotation, const Scale& inScale);

	template <typename T>
	APEIROGON_API bool				FindAllActors(std::vector<ActorPtr>& outActors);
	APEIROGON_API bool				FindActor(const int64 inGameObjectID, ActorPtr& outActor);
	APEIROGON_API bool				FindActors(std::vector<int64> inGameObjectIDs, std::vector<ActorPtr>& outActors);
	APEIROGON_API bool				FindActors(const FVector& inFindLocation, const float& inRadius, const uint8& inActorType, std::vector<ActorPtr>& outActors, const uint32& inMaxSize = INFINITE);
	APEIROGON_API bool				FindActors(BoxTrace& inBoxTrace, const uint8& inActorType, std::vector<ActorPtr>& outActors, const uint32& inMaxSize = INFINITE);
	APEIROGON_API bool				FindActors(SphereTrace& inSphereTrace, const uint8& inActorType, std::vector<ActorPtr>& outActors, const uint32& inMaxSize = INFINITE);

	APEIROGON_API bool				FindPlayer(const int64 inRemoteID, RemoteClientPtr& outRemoteClinet);
	APEIROGON_API bool				InsertPlayer(const int64 inRemoteID, RemoteClientPtr& inRemoteClinet);
	APEIROGON_API bool				DeletePlayer(const int64 inRemoteID);

	APEIROGON_API bool				FindObstructionIntersection(LineTrace& inLineTrace, const uint8& inActorType, std::vector<FVector>& outIntersection, const uint32& inMaxSize = INFINITE);

	APEIROGON_API bool				DestroyAllActor();
	APEIROGON_API bool				DestroyActors(const uint8 inActorType);
	APEIROGON_API bool				DestroyActor(const int64 inGameObjectID);

	APEIROGON_API bool				IsValidActor(const int64 inGameObjectID);

	APEIROGON_API void				SendWorldPlayers(SendBufferPtr inSendBuffer);

public:
	APEIROGON_API WorldPtr			GetWorldPtr()		{ return std::static_pointer_cast<World>(shared_from_this()); }
	APEIROGON_API WorldRef			GetWorldRef()		{ return std::static_pointer_cast<World>(shared_from_this()); }
	APEIROGON_API DataManagerPtr	GetDatas()			{ return GetTaskManagerRef().lock()->GetService()->GetDataManager(); }
	APEIROGON_API const int64		GetWorldTime()		{ return GetTaskManagerRef().lock()->GetService()->GetServiceTimeStamp(); }
	APEIROGON_API const int64		GetNextWorldTime()	{ return GetTaskManagerRef().lock()->GetService()->GetNextServiceTimeStamp(); }

protected:
	std::map<int64, RemoteClientPtr>	mWorldPlayers;
	std::map<int64, ActorPtr>			mWorldActors;
	KDTree								mWorldObserver;
	KDTree								mWorldObstruction;
};

template<typename T>
inline ActorPtr World::SpawnActor(GameObjectRef inOwner, const Location& inLocation, const Rotation& inRotation, const Scale& inScale)
{
	TaskManagerPtr taskManager = GetTaskManagerRef().lock();
	if (nullptr == taskManager)
	{
		return nullptr;
	}

	ActorPtr actor = std::static_pointer_cast<Actor>(std::make_shared<T>());
	if (nullptr == actor)
	{
		return nullptr;
	}
	actor->SetOwner(inOwner);
	actor->SetWorld(GetWorldRef());
	actor->SetLocation(inLocation);
	actor->SetRotation(inRotation);
	actor->SetScale(inScale);

	//GameObjectPtr gameObject = actor->GetGameObjectPtr();
	//taskManager->PushTask(gameObject);

	const int64 objectID = actor->GetGameObjectID();
	std::pair<int64, ActorPtr> newObject = std::make_pair(objectID, actor);
	auto result = mWorldActors.insert(newObject);

	//mWorldObserver.InsertNode(actor->GetActorRef());
	return actor;
}

template<typename T>
inline bool World::FindAllActors(std::vector<ActorPtr>& outActors)
{
	for (auto actor : mWorldActors)
	{
		std::shared_ptr<T> findActor = std::static_pointer_cast<T>(actor.second);
		if (nullptr != findActor)
		{
			outActors.push_back(findActor);
		}
	}

	return outActors.size() != 0;
}