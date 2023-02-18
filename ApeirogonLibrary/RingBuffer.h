#pragma once

class RingBuffer
{
private:
	enum class Default
	{
		BufferSize = 0x10000,
	};

public:
	APEIROGON_API RingBuffer(void);
	APEIROGON_API ~RingBuffer();

	RingBuffer(const RingBuffer& ringBuffer) = delete;
	RingBuffer& operator=(const RingBuffer& ringBuffer) = delete;

public:
	void			SetBufferSize(const uint32 bufferSize = static_cast<uint32>(Default::BufferSize));
	void			Clean();

public:
	APEIROGON_API void			MoveRear(const uint32 size);
	APEIROGON_API void			MoveFront(const uint32 len);

	inline uint32				GetWritePos() const;
	inline uint32				GetReadPos() const;

	APEIROGON_API inline BYTE*	GetBuffer();
	APEIROGON_API inline BYTE*	GetWriteBuffer();
	APEIROGON_API inline BYTE*	GetReadBuffer();

	inline uint32				GetUsedSize() const;
	inline uint32				GetTotalSize() const;
	inline uint32				GetFreeSize()	const;
	inline uint32				GetRecvMaxSize() const;

private:
	uint32	mWritePos;
	uint32	mReadPos;
	uint32	mBufferSize;
	BYTE*	mBuffer;
};

class SendRingBuffer : public RingBuffer
{
public:
	APEIROGON_API SendRingBuffer();
	APEIROGON_API virtual ~SendRingBuffer();

	SendRingBuffer(const SendRingBuffer& ringBuffer) = delete;
	SendRingBuffer& operator=(const SendRingBuffer& ringBuffer) = delete;

public:
	APEIROGON_API SendBufferPtr	Writer(const uint32 size);
	APEIROGON_API void Reserve(const uint32 len);

private:
	FastSpinLock mFastSpinLock;
};

class RecvRingBuffer : public RingBuffer
{
public:
	APEIROGON_API RecvRingBuffer();
	APEIROGON_API virtual ~RecvRingBuffer();

	RecvRingBuffer(const RecvRingBuffer& ringBuffer) = delete;
	RecvRingBuffer& operator=(const RecvRingBuffer& ringBuffer) = delete;

public:
	APEIROGON_API uint32	Enqueue(const BYTE* data, const uint32 size);
	APEIROGON_API uint32	Dequeue(BYTE* dest, const uint32 len);
	APEIROGON_API uint32	Peek(BYTE* dest, const uint32 len);
};