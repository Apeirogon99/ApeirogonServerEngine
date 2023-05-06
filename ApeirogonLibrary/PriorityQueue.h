#pragma once

//MIN HEAP
template<typename Node>
class PriorityQueue
{
public:
	PriorityQueue()
	{
	}
	virtual ~PriorityQueue()
	{
	}

public:
	APEIROGON_API bool Enqueue(const Node& inNode)
	{
		FastLockGuard lockGuard(mLock);
		mNodes.push(inNode);
		return true;
	}

	APEIROGON_API bool Dequeue()
	{
		FastLockGuard lockGuard(mLock);

		if (mNodes.empty())
		{
			return false;
		}

		mNodes.pop();
		return true;
	}

	APEIROGON_API bool Dequeue(Node& outNode)
	{
		FastLockGuard lockGuard(mLock);

		if (mNodes.empty())
		{
			return false;
		}

		outNode = std::move(mNodes.top());
		mNodes.pop();
		return true;
	}

	APEIROGON_API bool DequeueAll(std::vector<Node>& outNodes)
	{
		FastLockGuard lockGuard(mLock);

		if (mNodes.empty())
		{
			return false;
		}

		while (!mNodes.empty())
		{
			Node topNode = mNodes.top();
			mNodes.pop();

			outNodes.push_back(std::move(topNode));
		}

		return true;
	}

	APEIROGON_API bool Peek(Node& outNode)
	{
		FastLockGuard lockGuard(mLock);

		if (mNodes.empty())
		{
			return false;
		}

		outNode = mNodes.top();
		return true;
	}

	APEIROGON_API void Claer()
	{
		FastLockGuard lockGuard(mLock);
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
	FastSpinLock				mLock;
	std::priority_queue<Node>	mNodes;
};