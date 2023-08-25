#include "pch.h"
#include "World.h"

World::World(const WCHAR* inName) : GameObject(inName), mWorldObserver(1024)
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

bool World::FindActors(std::vector<int64> inGameObjectIDs, std::vector<ActorPtr>& outActors)
{
	for (const int64& gameObjectID : inGameObjectIDs)
	{
		auto findPos = mWorldActors.find(gameObjectID);
		if (findPos != mWorldActors.end())
		{
			outActors.push_back(findPos->second);
		}
	}

	return outActors.size();
}

bool World::FindActors(const FVector& inFindLocation, const float& inRadius, const uint8& inActorType, std::vector<ActorPtr>& outActors)
{
	std::vector<int64> actorGameObjectIDs;
	bool result = this->mWorldObserver.FindNodes(inFindLocation, inRadius, inActorType, actorGameObjectIDs);
	if (!result)
	{
		return false;
	}

	for (const int64& gameObjectID : actorGameObjectIDs)
	{
		auto findPos = mWorldActors.find(gameObjectID);
		if (findPos != mWorldActors.end())
		{
			outActors.push_back(findPos->second);
		}
	}
	
	return outActors.size();
}

bool World::FindActors(BoxTrace& inBoxTrace, const uint8& inActorType, std::vector<ActorPtr>& outActors, const uint32& inMaxSize)
{
	std::vector<int64> actorGameObjectIDs;
	bool result = this->mWorldObserver.SearchNodes(inBoxTrace, inActorType, actorGameObjectIDs, inMaxSize);
	if (!result)
	{
		return false;
	}

	for (const int64& gameObjectID : actorGameObjectIDs)
	{
		auto findPos = mWorldActors.find(gameObjectID);
		if (findPos != mWorldActors.end())
		{
			outActors.push_back(findPos->second);
		}
	}

	return outActors.size();
}

bool World::FindActors(SphereTrace& inSphereTrace, const uint8& inActorType, std::vector<ActorPtr>& outActors, const uint32& inMaxSize)
{
	std::vector<int64> actorGameObjectIDs;
	bool result = this->mWorldObserver.SearchNodes(inSphereTrace, inActorType, actorGameObjectIDs, inMaxSize);
	if (!result)
	{
		return false;
	}

	for (const int64& gameObjectID : actorGameObjectIDs)
	{
		auto findPos = mWorldActors.find(gameObjectID);
		if (findPos != mWorldActors.end())
		{
			outActors.push_back(findPos->second);
		}
	}

	return outActors.size();
}

bool World::DestroyAllActor()
{

	TaskManagerPtr taskManager = GetTaskManagerRef().lock();
	if (nullptr == taskManager)
	{
		return false;
	}

	for (auto player = mWorldPlayers.begin(); player != mWorldPlayers.end();)
	{

		taskManager->DestroyGameObject(player->second->GetRemotePlayer()->GetGameObjectPtr());

		const PlayerMonitors& playerMonitors = player->second->GetPlayerMonitors();
		for (auto playerMonitor = playerMonitors.begin(); playerMonitor != playerMonitors.end();)
		{
			playerMonitor->get()->ReleasePlayerViewer(player->second);
			player->second->ReleasePlayerMonitor(*playerMonitor++);
		}

		const ActorMonitors& actorMonitors = player->second->GetActorMonitors();
		for (auto actorMonitor = actorMonitors.begin(); actorMonitor != actorMonitors.end();)
		{
			actorMonitor->get()->ReleasePlayerViewer(player->second);
			player->second->ReleaseActorMonitor(*actorMonitor++);
		}

		mWorldPlayers.erase(player++->first);
	}

	for (auto actor = mWorldActors.begin(); actor != mWorldActors.end();)
	{

		GameObjectPtr actorGameObject = actor->second->GetGameObjectPtr();
		taskManager->ReleaseTask(actorGameObject);

		const PlayerViewer& playerViewers = actor->second->GetPlayerViewers();
		for (auto playerViewer = playerViewers.begin(); playerViewer != playerViewers.end();)
		{
			playerViewer->get()->ReleaseActorMonitor(actor->second);
			actor->second->ReleasePlayerViewer(*playerViewer++);
		}

		mWorldActors.erase(actor++->first);
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
	ActorPtr actor = findPos->second;

	TaskManagerPtr taskManager = GetTaskManagerRef().lock();
	if (nullptr == taskManager)
	{
		return false;
	}

	GameObjectPtr actorGameObject = actor->GetGameObjectPtr();
	taskManager->ReleaseTask(actorGameObject);

	const PlayerViewer& playerViewers = actor->GetPlayerViewers();
	for (auto playerViewer = playerViewers.begin(); playerViewer != playerViewers.end();)
	{
		playerViewer->get()->ReleaseActorMonitor(actor);
		actor->ReleasePlayerViewer(*playerViewer++);
	}

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

void World::SendWorldPlayers(SendBufferPtr inSendBuffer)
{
	for (auto viewer : mWorldPlayers)
	{
		viewer.second->Send(inSendBuffer);
	}
}
