#include "pch.h"
#include "TaskManager.h"

TaskManager::TaskManager()
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

void TaskManager::PushTask(GameObjectPtr& inGameObject)
{
	inGameObject->Initialization();
	std::pair<const WCHAR*, GameObjectPtr> tempTask = std::make_pair(inGameObject->GetObjectName(), inGameObject);
	mGameObjects.insert(tempTask);
}

void TaskManager::ReleaseTask(GameObjectPtr& inGameObject)
{
	mGameObjects.erase(inGameObject->GetObjectName());
	inGameObject->Destroy();
	inGameObject->ClearTask();
}

GameObjectPtr TaskManager::FindTask(const WCHAR* inObjectName)
{
	auto object = mGameObjects.find(inObjectName);
	if (object->second == nullptr)
	{
		TaskManagerLog(L"[TaskManager::FindTask] Not found game object");
		return nullptr;
	}

	return object->second;
}

void TaskManager::TaskManagerLog(const WCHAR* log, ...)
{
	mService->GetLoggerManager()->WriteLog(log);
}
