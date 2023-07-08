#pragma once

class CompareNode {
public:
	APEIROGON_API bool operator()(const TaskNodePtr& node1, const TaskNodePtr& node2);
};

//MIN HEAP
class PriorityQueue
{
public:
	PriorityQueue() : mFastLock()
	{
	}
	virtual ~PriorityQueue()
	{
	}

public:
	APEIROGON_API bool Enqueue(const TaskNodePtr inNode)
	{
		FastLockGuard lockGuard(mFastLock);
		mNodes.push(inNode);
		return true;
	}

	APEIROGON_API bool Dequeue()
	{
		FastLockGuard lockGuard(mFastLock);

		if (mNodes.empty())
		{
			return false;
		}

		mNodes.pop();
		return true;
	}

	APEIROGON_API bool Dequeue(TaskNodePtr& outNode)
	{
		FastLockGuard lockGuard(mFastLock);

		if (mNodes.empty())
		{
			return false;
		}

		outNode = std::move(mNodes.top());
		mNodes.pop();
		return true;
	}

	APEIROGON_API bool DequeueAll(std::vector<TaskNodePtr>& outNodes)
	{
		FastLockGuard lockGuard(mFastLock);

		if (mNodes.empty())
		{
			return false;
		}

		while (!mNodes.empty())
		{
			TaskNodePtr topNode = mNodes.top();
			mNodes.pop();

			outNodes.push_back(std::move(topNode));
		}

		return true;
	}

	APEIROGON_API bool Peek(TaskNodePtr& outNode)
	{
		FastLockGuard lockGuard(mFastLock);

		if (mNodes.empty())
		{
			return false;
		}

		outNode = mNodes.top();
		return true;
	}

	APEIROGON_API void Claer()
	{
		FastLockGuard lockGuard(mFastLock);
		while (!mNodes.empty())
		{
			mNodes.pop();
		}
	}

	APEIROGON_API bool IsEmpty()
	{
		return mNodes.empty();
	}

	APEIROGON_API int32 Count()
	{
		return static_cast<int32>(mNodes.size());
	}


private:
	FastSpinLock				mFastLock;
	std::priority_queue<TaskNodePtr, std::vector<TaskNodePtr>, CompareNode> mNodes;
};