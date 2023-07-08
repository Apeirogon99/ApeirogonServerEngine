#pragma once

class TaskNode
{
public:
	APEIROGON_API TaskNode() : mPriority(0)
	{

	}
	
	APEIROGON_API ~TaskNode()
	{

	}

public:
	bool operator<(const TaskNode& inOtherNode) const
	{
		return this->mPriority < inOtherNode.mPriority;
	}

	template<typename T, typename Ret, typename... Args>
	void Init(const int64 inPriority, std::weak_ptr<T> inOwner, Ret(T::*inMemberFunc)(Args...), Args&&... inArgs)
	{
		this->mPriority = inPriority;
		mCallBack = [inOwner, inMemberFunc, inArgs...]()
		{
			std::shared_ptr<T> owner = inOwner.lock();
			if (owner)
			{
				(owner.get()->*inMemberFunc)(inArgs...);
			}
		};
	}

	void Execute()
	{
		mCallBack();
	}

	const int64 GetPriority() const
	{
		return mPriority;
	}

private:
	std::function<void()>	mCallBack;
	int64					mPriority;
};

class TaskQueue : public std::enable_shared_from_this<TaskQueue>
{
public:
	APEIROGON_API TaskQueue() : mFastSpinLock() {}
	APEIROGON_API ~TaskQueue() {}

	TaskQueue(TaskQueue&&) = delete;
	TaskQueue(const TaskQueue&) = delete;

	TaskQueue& operator=(TaskQueue&&) = delete;
	TaskQueue& operator=(const TaskQueue&) = delete;

public:

	template<typename T, typename Ret, typename... Args>
	APEIROGON_API bool PushTask(const int64 inPriority, Ret(T::*inMemberFunc)(Args...), Args... inArgs)
	{
		std::weak_ptr<T> owner = std::static_pointer_cast<T>(shared_from_this());
		TaskNodePtr newTaskNode = std::make_shared<TaskNode>();
		newTaskNode->Init(inPriority, owner, inMemberFunc, std::forward<Args>(inArgs)...);

		FastLockGuard lockGaurd(mFastSpinLock);
		bool result = mTaskQueue.Enqueue(newTaskNode);

		return result;
	}

	APEIROGON_API void ClearTask()
	{
		FastLockGuard lockGaurd(mFastSpinLock);
		int32 count = mTaskQueue.Count();
		for (int32 task = 0; task < count; ++task)
		{
			TaskNodePtr taskNode;
			mTaskQueue.Dequeue(taskNode);
			taskNode.reset();
		}
	}

	APEIROGON_API bool Execute(const int64 inServiceTimeStamp)
	{
		std::vector<TaskNodePtr> TaskNodes;
		{
			FastLockGuard lockGaurd(mFastSpinLock);
			if (mTaskQueue.IsEmpty())
			{
				return true;
			}

			while (true)
			{
				TaskNodePtr peekTaskNode;
				if (false == mTaskQueue.Peek(peekTaskNode))
				{
					break;
				}

				if (peekTaskNode->GetPriority() > inServiceTimeStamp)
				{
					break;
				}

				TaskNodes.emplace_back(std::move(peekTaskNode));
				mTaskQueue.Dequeue();
			}
		}

		if (TaskNodes.empty())
		{
			return true;
		}

		for (int32 curTaskNode = 0; curTaskNode < TaskNodes.size(); ++curTaskNode)
		{
			TaskNodes[curTaskNode]->Execute();
		}

		return true;
	}

	APEIROGON_API const int64 TaskTop()
	{
		TaskNodePtr taskNode;
		if (true == mTaskQueue.Peek(taskNode))
		{
			return taskNode->GetPriority();
		}

		return 0;
	}

	APEIROGON_API const int32 TaskCount()
	{
		return mTaskQueue.Count();
	}

private:
	FastSpinLock				mFastSpinLock;
	PriorityQueue				mTaskQueue;
};

