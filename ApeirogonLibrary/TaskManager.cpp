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

	return true;
}

void TaskManager::Shutdown()
{
	mService.reset();
}

void TaskManager::ProcessTask()
{
	const int64 nextTick = mService->GetServiceTimeStamp();
	for (TaskQueue& task : mTaskQueues)
	{
		task.Execute(nextTick);
	}
}

void TaskManager::Tick()
{

}
