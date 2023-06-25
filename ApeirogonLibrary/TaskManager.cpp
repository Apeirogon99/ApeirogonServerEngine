#include "pch.h"
#include "TaskManager.h"

TaskManager::TaskManager() : mGameObjectCount(0)
{
}

TaskManager::~TaskManager()
{
}

bool TaskManager::Prepare(ServicePtr service)
{
	mService = service;
	if (mService == nullptr)
	{
		return false;
	}

	Init();

	TaskManagerLog(L"[TaskManager::Prepare()] Task manager success prepare\n");

	return true;
}

void TaskManager::Shutdown()
{
	for (auto& task : mGameObjects)
	{
		task.second->Destroy();
		task.second->ClearTask();
	}

	mGameObjects.clear();
	mService.reset();

	TaskManagerLog(L"[TaskManager::Shutdown()] Task manager success shutdown\n");
}

void TaskManager::ProcessTask(const int64 inServiceTimeStamp)
{
	for (auto& task : mGameObjects)
	{
		task.second->Execute(inServiceTimeStamp);
	}
}

void TaskManager::Tick()
{
	for (auto& task : mGameObjects)
	{
		task.second->Tick();
	}
}

void TaskManager::CreateGameObject(GameObjectPtr inGameObject)
{
	const int64 curGameObjectID = NextGameObjectNumber();

	inGameObject->Initialization();
	inGameObject->SetGameObjectID(curGameObjectID);
}

void TaskManager::DestroyGameObject(GameObjectPtr inGameObject)
{
	inGameObject->Destroy();
	inGameObject->ClearTask();
}

void TaskManager::PushTask(GameObjectPtr inGameObject)
{
	const int64 curGameObjectID = NextGameObjectNumber();

	inGameObject->Initialization();
	inGameObject->SetGameObjectID(curGameObjectID);

	std::pair<int64, GameObjectPtr> tempTask = std::make_pair(curGameObjectID, inGameObject);
	mGameObjects.insert(tempTask);
}

void TaskManager::ReleaseTask(GameObjectPtr inGameObject)
{
	mGameObjects.erase(inGameObject->GetGameObjectID());

	inGameObject->Destroy();
	inGameObject->ClearTask();
}

bool TaskManager::FindTask(const int64 inGameObjectID, GameObjectPtr& outGameObject)
{
	auto object = mGameObjects.find(inGameObjectID);
	if (object->second == nullptr)
	{
		TaskManagerLog(L"[TaskManager::FindTask] Not found game object");
		return false;
	}

	outGameObject = object->second;
	return true;
}

bool TaskManager::FindTask(const WCHAR* inGameObjectName, GameObjectPtr& outGameObject)
{
	for (auto gameObject : mGameObjects)
	{
		if (gameObject.second)
		{
			if (wcscmp(gameObject.second->GetGameObjectName(), inGameObjectName) == 0)
			{
				outGameObject = gameObject.second;
				return true;
			}
		}
	}

	TaskManagerLog(L"[TaskManager::FindTask] Not found game object");
	return false;
}

const int64 TaskManager::NextGameObjectNumber()
{
	const int64 nextGameObject = mGameObjectCount++;
	return nextGameObject;
}

void TaskManager::TaskManagerLog(const WCHAR* log, ...)
{
	mService->GetLoggerManager()->WriteLog(log);
}
