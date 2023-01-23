#pragma once

class SendQueue
{
	enum class Default
	{
		DO_ONCE_POPALL_COUNT = 100,
		MAX_QUEUE_SIZE = 200,
	};

public:
	APEIROGON_API SendQueue();
	APEIROGON_API ~SendQueue();

	SendQueue(const SendQueue& queue) = delete;
	SendQueue& operator=(const SendQueue& queue) = delete;

public:
	APEIROGON_API void	Push(SendBufferPtr sendBuffer);
	APEIROGON_API void	PopAll(std::vector<SendBufferPtr>& sendBuffers);
	APEIROGON_API bool	IsEmpty();
protected:
	bool				IsFull();

private:
	FastSpinLock	mFastSpinLock;
	uint32			mQueueSize;
	uint32			mFront;
	uint32			mRear;
	std::array<SendBufferPtr, static_cast<size_t>(Default::MAX_QUEUE_SIZE)> mBuffer;
};

