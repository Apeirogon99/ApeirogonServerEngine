#pragma once

class NetworkTaskNode
{
public:
	APEIROGON_API NetworkTaskNode() : mPriority(0)
	{

	}
	
	APEIROGON_API ~NetworkTaskNode()
	{

	}

public:
	bool operator<(const NetworkTaskNode& inOtherNode) const
	{
		return this->mPriority > inOtherNode.mPriority;
	}

	template<typename T, typename Ret, typename... Args>
	void Init(const int64 inPriority, std::weak_ptr<T> inOwner, Ret(T::*inMemberFunc)(Args...), Args&&... inArgs)
	{
		mPriority = inPriority;
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

class NetworkQueue : public std::enable_shared_from_this<NetworkQueue>
{
public:
	APEIROGON_API NetworkQueue() {}
	APEIROGON_API ~NetworkQueue() {}

	NetworkQueue(NetworkQueue&&) = delete;
	NetworkQueue(const NetworkQueue&) = delete;

	NetworkQueue& operator=(NetworkQueue&&) = delete;
	NetworkQueue& operator=(const NetworkQueue&) = delete;

public:

	template<typename T, typename Ret, typename... Args>
	APEIROGON_API bool PushTask(const int64 inPriority, Ret(T::*inMemberFunc)(Args...), Args... inArgs)
	{
		std::weak_ptr<T> owner = std::static_pointer_cast<T>(shared_from_this());
		NetworkTaskNodePtr newTaskNode = std::make_shared<NetworkTaskNode>();
		newTaskNode->Init(inPriority, owner, inMemberFunc, std::forward<Args>(inArgs)...);
		return mTaskQueue.Enqueue(newTaskNode);
	}

	APEIROGON_API void ClearTask()
	{
		int32 count = mTaskQueue.Count();
		for (int32 task = 0; task < count; ++task)
		{
			NetworkTaskNodePtr taskNode;
			mTaskQueue.Dequeue(taskNode);
			taskNode.reset();
		}
	}

	APEIROGON_API bool Execute(const int64 inServiceTimeStamp)
	{
		std::vector<NetworkTaskNodePtr> TaskNodes;
		while (true)
		{
			NetworkTaskNodePtr peekTaskNode;
			if (false == mTaskQueue.Peek(peekTaskNode))
			{
				break;
			}

			if (peekTaskNode->GetPriority() > inServiceTimeStamp)
			{
				break;
			}

			TaskNodes.emplace_back(peekTaskNode);
			mTaskQueue.Dequeue();
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

private:
	PriorityQueue<NetworkTaskNodePtr> mTaskQueue;
};

