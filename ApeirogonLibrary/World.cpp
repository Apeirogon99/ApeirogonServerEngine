#include "pch.h"
#include "World.h"

World::World(const WCHAR* inName, TaskManagerRef inTaskManager) : GameObject(inName), mTaskManager(inTaskManager)
{
}

World::~World()
{
}

template<typename T>
inline ActorPtr World::SpawnActor(const Location& inLocation, const Rotation& inRotation, const Scale& inScale)
{
	TaskManagerPtr taskManager = mTaskManager.lock();
	if (nullptr == taskManager)
	{
		return nullptr;
	}

	ActorPtr actor = std::static_pointer_cast<Actor>(std::make_shared<T>());
	if (nullptr == actor)
	{
		return nullptr;
	}
	GameObjectPtr gameObject = actor->GetGameObjectPtr();
	taskManager->PushTask(gameObject);

	actor->SetWorld(GetWorldRef());
	actor->SetLocation(inLocation);
	actor->SetRotation(inRotation);
	actor->SetScale(inScale);

	const int64 objectID = actor->GetGameObjectID();
	std::pair<int64, ActorPtr> newObject = std::make_pair(objectID, actor);
	auto result = mWorldActors.insert(newObject);
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

bool World::FindActor(const int64 inGameObjectID, ActorPtr& outActor)
{
	auto findPos = mWorldActors.find(inGameObjectID);
	if (findPos == mWorldActors.end())
	{
		return false;
	}

	outActor = findPos->second;
	return true;
}

bool World::DestroyAllActor()
{
	for (auto actor : mWorldActors)
	{
		DestroyActor(actor.second->GetGameObjectID());
	}

	return true;
}

bool World::DestroyActor(const int64 inGameObjectID)
{
	auto findPos = mWorldActors.find(inGameObjectID);
	if (findPos == mWorldActors.end())
	{
		return false;
	}

	TaskManagerPtr taskManager = mTaskManager.lock();
	if (nullptr == taskManager)
	{
		return false;
	}

	GameObjectPtr actorGameObject = findPos->second->GetGameObjectPtr();
	taskManager->ReleaseTask(actorGameObject);

	if (0 == mWorldActors.erase(inGameObjectID))
	{
		return false;
	}

	return true;
}

bool World::IsValidActor(const int64 inGameObjectID)
{
	auto findPos = mWorldActors.find(inGameObjectID);
	if (findPos == mWorldActors.end())
	{
		return false;
	}

	return true;
}
