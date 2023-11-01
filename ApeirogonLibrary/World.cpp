#include "pch.h"
#include "World.h"

World::World(const WCHAR* inName) : GameObject(inName), mWorldObserver(1024), mWorldObstruction(1024)
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

bool World::FindActors(const FVector& inFindLocation, const float& inRadius, const uint8& inActorType, std::vector<ActorPtr>& outActors, const uint32& inMaxSize)
{
	std::vector<int64> actorGameObjectIDs;
	bool result = this->mWorldObserver.SearchNodes(inFindLocation, inRadius, inActorType, actorGameObjectIDs);
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

bool World::FindPlayer(const int64 inRemoteID, RemoteClientPtr& outRemoteClinet)
{
	auto findPos = mWorldPlayers.find(inRemoteID);
	if (findPos == mWorldPlayers.end())
	{
		return false;
	}

	outRemoteClinet = findPos->second;
	return true;
}

bool World::InsertPlayer(const int64 inRemoteID, RemoteClientPtr& inRemoteClinet)
{
	auto findPos = mWorldPlayers.find(inRemoteID);
	if (findPos != mWorldPlayers.end())
	{
		return false;
	}

	mWorldPlayers.insert(std::make_pair(inRemoteID, inRemoteClinet));
	return true;
}

bool World::DeletePlayer(const int64 inRemoteID)
{
	auto findPos = mWorldPlayers.find(inRemoteID);
	if (findPos == mWorldPlayers.end())
	{
		return false;
	}

	mWorldPlayers.erase(inRemoteID);
	return true;
}

bool World::FindObstructionIntersection(LineTrace& inLineTrace, const uint8& inActorType, std::vector<FVector>& outIntersection, const uint32& inMaxSize)
{

	bool result = this->mWorldObstruction.SearchNodes(inLineTrace, inActorType, outIntersection, inMaxSize);
	if (!result)
	{
		return false;
	}

	return outIntersection.size();
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

bool World::DestroyActors(const uint8 inActorType)
{
	TaskManagerPtr taskManager = GetTaskManagerRef().lock();
	if (nullptr == taskManager)
	{
		return false;
	}

	for (auto actor = mWorldActors.begin(); actor != mWorldActors.end();)
	{
		if (actor->second)
		{
			if (actor->second->GetActorType() == inActorType)
			{
				GameObjectPtr actorGameObject = actor->second->GetGameObjectPtr();
				taskManager->ReleaseTask(actorGameObject);

				const PlayerViewer& playerViewers = actor->second->GetPlayerViewers();
				for (auto playerViewer = playerViewers.begin(); playerViewer != playerViewers.end();)
				{
					if (*playerViewer)
					{
						playerViewer->get()->ReleaseActorMonitor(actor->second);
						actor->second->ReleasePlayerViewer(*playerViewer++);
					}
				}

				mWorldActors.erase(actor++->first);
			}
			else
			{
				++actor;
			}
		}
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
