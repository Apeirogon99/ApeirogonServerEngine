#pragma once

class SendQueue
{
	enum class Default : int32
	{
		DO_ONCE_POPALL_COUNT = 100,
		MAX_QUEUE_SIZE = 0xffff,
	};

public:
	APEIROGON_API SendQueue();
	APEIROGON_API ~SendQueue();

private:
	SendQueue(const SendQueue&) = delete;
	SendQueue(SendQueue&&) = delete;
	SendQueue& operator=(const SendQueue&) = delete;
	SendQueue& operator=(SendQueue&&) = delete;

public:
	APEIROGON_API void	Clear();
	APEIROGON_API void	Push(SendBufferPtr& inSendBuffer);
	APEIROGON_API void	Pop(SendBufferPtr& OutSendBuffer);
	APEIROGON_API void	Pop(std::vector<SendBufferPtr> OutSendBuffers);

public:
	APEIROGON_API bool	IsEmpty();
	APEIROGON_API bool	IsFull();

private:
	CircularQueue<SendBufferPtr>	mSendQueue;
};

