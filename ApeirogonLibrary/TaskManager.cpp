#include "pch.h"
#include "TaskManager.h"

TaskManager::TaskManager() : mGameObjectIDCount(INVALID_GAMEOBJECT_ID), mTaskProcessTimeStamp(L"Task"), mTickProcessTimeStamp(L"Tick"), mCurrentTickGameObjectCount(0), mCurrentGameObjectCount(0)
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
	for (auto task = mGameObjects.begin(); task != mGameObjects.end();)
	{
		this->DestroyGameObject(task->second);
		this->mGameObjects.erase(task++);
	}

	TaskManagerLog(L"[TaskManager::Shutdown()] Task manager success shutdown\n");

	mGameObjects.clear();
	mService.reset();
}

int64 TaskManager::ProcessTask(const int64 inServiceTimeStamp)
{
	mTaskProcessTimeStamp.StartTimeStamp();
	for (auto task = mGameObjects.begin(); task != mGameObjects.end();)
	{
		if (false == task->second->Execute(inServiceTimeStamp))
		{
			this->mGameObjects.erase(task++);
		}
		else
		{
			++task;
		}
	}
	return mTaskProcessTimeStamp.GetTimeStamp();
}

int64 TaskManager::Tick(const int64 inTickTime)
{
	mTickProcessTimeStamp.StartTimeStamp();
	for (auto& task : mGameObjects)
	{
		task.second->Tick(inTickTime + mTickProcessTimeStamp.GetTimeStamp());
	}
	return mTickProcessTimeStamp.GetTimeStamp();
}

void TaskManager::CreateGameObject(GameObjectPtr inGameObject)
{
	const int64 curGameObjectID = NextGameObjectNumber();

	inGameObject->SetTaskManagerRef(weak_from_this());
	inGameObject->Initialization();
	inGameObject->SetGameObjectID(curGameObjectID);

	mCurrentGameObjectCount++;
}

void TaskManager::DestroyGameObject(GameObjectPtr inGameObject)
{
	inGameObject->Destroy();
	inGameObject->ClearTask();

	mCurrentGameObjectCount--;
}

void TaskManager::PushTask(GameObjectPtr inGameObject)
{

	CreateGameObject(inGameObject);
	TaskManagerLog(L"[TaskManager::PushTask][%lld] Push Task %ws\n", inGameObject->GetGameObjectID(), inGameObject->GetGameObjectName());

	const int64 objectID = inGameObject->GetGameObjectID();
	std::pair<int64, GameObjectPtr> tempTask = std::make_pair(objectID, inGameObject);
	mGameObjects.insert(tempTask);

	mCurrentTickGameObjectCount++;
}

void TaskManager::ReleaseTask(GameObjectPtr inGameObject)
{
	const WCHAR* gameObjectName = inGameObject->GetGameObjectName();

	TaskManagerLog(L"[TaskManager::ReleaseTask][%lld] Release Task %ws\n", inGameObject->GetGameObjectID(), gameObjectName);
	DestroyGameObject(inGameObject);

	mCurrentTickGameObjectCount--;

}

bool TaskManager::FindTask(const int64 inGameObjectID, GameObjectPtr& outGameObject)
{
	auto object = mGameObjects.find(inGameObjectID);
	if (object->second == nullptr)
	{
		TaskManagerLog(L"[TaskManager::FindTask] Not found game object\n");
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

	TaskManagerLog(L"[TaskManager::FindTask] Not found game object\n");
	return false;
}

const int64 TaskManager::NextGameObjectNumber()
{
	const int64 nextGameObject = ++mGameObjectIDCount;
	return nextGameObject;
}

void TaskManager::TaskManagerLog(const WCHAR* log, ...)
{
	mService->GetLoggerManager()->WriteLog(log);
}
