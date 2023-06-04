#pragma once
class TaskManager
{
public:
	APEIROGON_API TaskManager();
	APEIROGON_API virtual ~TaskManager();

private:
	TaskManager(const TaskManager&) = delete;
	TaskManager(TaskManager&&) = delete;

	TaskManager& operator=(const TaskManager&) = delete;
	TaskManager& operator=(TaskManager&&) = delete;

public:
	bool					Prepare(ServicePtr service);
	void					Shutdown();

public:
	void ProcessTask();
	void Tick();

protected:

private:
	ServicePtr				mService;

	std::vector<TaskQueue>	mTaskQueues;
};

