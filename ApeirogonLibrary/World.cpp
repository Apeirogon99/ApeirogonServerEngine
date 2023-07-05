#include "pch.h"
#include "World.h"

World::World(const WCHAR* inName) : GameObject(inName)
{
}

World::~World()
{
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

	TaskManagerPtr taskManager = GetTaskManagerRef().lock();
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
