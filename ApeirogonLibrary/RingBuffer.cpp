#include "pch.h"
#include "RingBuffer.h"

RingBuffer::RingBuffer(void) : mBuffer(nullptr), mBufferSize(0), mWritePos(0), mReadPos(0)
{
	
}

RingBuffer::~RingBuffer()
{
	if (nullptr != mBuffer)
	{
		delete[] mBuffer;
	}
	mBuffer = nullptr;

}

void RingBuffer::SetBufferSize(const uint32 bufferSize)
{
	mBufferSize = bufferSize;
	if (mBufferSize < 0)
	{
		return;
	}

	mBuffer = new BYTE[mBufferSize];

	Clean();
}

void RingBuffer::Clean()
{
	if (mBuffer)
	{
		mWritePos = 0;
		mReadPos = 0;
		::memset(mBuffer, NULL, mBufferSize);
	}
}


void RingBuffer::MoveRear(const uint32 size)
{
	mWritePos = (mWritePos + size) % mBufferSize;
}

void RingBuffer::MoveFront(const uint32 len)
{
	mReadPos = (mReadPos + len) % mBufferSize;
}

inline uint32 RingBuffer::GetWritePos() const
{
	return mWritePos;
}

inline uint32 RingBuffer::GetReadPos() const
{
	return mReadPos;
}

inline BYTE* RingBuffer::GetBuffer()
{
	return mBuffer;
}

BYTE* RingBuffer::GetWriteBuffer()
{
	return &mBuffer[mWritePos];
}

BYTE* RingBuffer::GetReadBuffer()
{
	return &mBuffer[mReadPos];
}

inline uint32 RingBuffer::GetUsedSize() const
{
	if (GetWritePos() == GetReadPos())
	{
		return 0;
	}
	else if (GetWritePos() > GetReadPos())
	{
		return GetWritePos() - GetReadPos();
	}
	else if (GetWritePos() < GetReadPos())
	{
		return GetTotalSize() - (GetReadPos() - GetWritePos());
	}
	
	wprintf(L"RingBuffer::GetAllocated()");
	return mBufferSize;
}

inline uint32 RingBuffer::GetTotalSize() const
{
	return mBufferSize;
}

inline uint32 RingBuffer::GetFreeSize() const
{
	return GetTotalSize() - GetUsedSize();
}

inline uint32 RingBuffer::GetRecvMaxSize() const
{
	uint32 maxSize = GetTotalSize() - GetWritePos();
	return maxSize;
}

//
//SendRingBuffer
//

SendRingBuffer::SendRingBuffer()
{
}

SendRingBuffer::~SendRingBuffer()
{
}

SendBufferPtr SendRingBuffer::Writer(const uint32 size)
{
	FastLockGuard lockGuard(mFastSpinLock);
	if (GetFreeSize() < size)
	{
		return 0;
	}

	const uint32 PrevWritePos = GetWritePos();
	const bool IsOverBuffer = (PrevWritePos + size) > GetTotalSize() ? true : false;
	if (IsOverBuffer)
	{
		const uint32 OverLen = (PrevWritePos + size) - GetTotalSize();
		const uint32 LessLen = size - OverLen;

		MoveRear(LessLen);
	}
	else
	{
		MoveRear(size);
	}

	return std::make_shared<SendBuffer>(GetWriteBuffer(), size);
}

void SendRingBuffer::Reserve(const uint32 len)
{
	FastLockGuard lockGuard(mFastSpinLock);
	if (GetUsedSize() < len)
	{
		return;
	}

	const uint32 PrevReadPos = GetReadPos();
	const bool IsOverBuffer = (PrevReadPos + len) > GetTotalSize() ? true : false;
	if (IsOverBuffer)
	{
		const uint32 OverLen = (PrevReadPos + len) - GetTotalSize();
		const uint32 LessLen = len - OverLen;

		MoveFront(LessLen + (LessLen + OverLen));
	}
	else
	{
		MoveFront(len);
	}
}

//
//RecvRingBuffer
//

RecvRingBuffer::RecvRingBuffer()
{
}

RecvRingBuffer::~RecvRingBuffer()
{
}

uint32 RecvRingBuffer::Enqueue(const BYTE* data, const uint32 size)
{
	if (GetFreeSize() < size)
	{
		return 0;
	}

	const uint32 PrevWritePos = GetWritePos();
	const bool IsOverBuffer = (PrevWritePos + size) > GetTotalSize() ? true : false;
	if (IsOverBuffer)
	{
		const uint32 OverLen = (PrevWritePos + size) - GetTotalSize();
		const uint32 LessLen = size - OverLen;

		::memcpy(&GetBuffer()[PrevWritePos], &data[0], LessLen);
		::memcpy(&GetBuffer()[0], &data[LessLen], OverLen);
		MoveRear(OverLen + LessLen);
		//mWritePos = (mWritePos + OverLen + LessLen) % mBufferSize;
	}
	else
	{
		::memcpy(&GetBuffer()[PrevWritePos], data, size);
		MoveRear(size);
		//mWritePos = (mWritePos + size) % mBufferSize;
	}

	return size;
}

uint32 RecvRingBuffer::Dequeue(BYTE* dest, const uint32 len)
{
	if (GetUsedSize() < len)
	{
		return 0;
	}

	const uint32 PrevReadPos = GetReadPos();
	const bool IsOverBuffer = (PrevReadPos + len) > GetTotalSize() ? true : false;
	if (IsOverBuffer)
	{
		const uint32 OverLen = (PrevReadPos + len) - GetTotalSize();
		const uint32 LessLen = len - OverLen;

		::memcpy(dest, &GetBuffer()[PrevReadPos], LessLen);
		::memcpy(&dest[LessLen], &GetBuffer()[0], OverLen);
		MoveFront(OverLen + LessLen);
		//mReadPos = (mReadPos + OverLen + LessLen) % mBufferSize;
	}
	else
	{
		::memcpy(dest, &GetBuffer()[PrevReadPos], len);
		MoveFront(len);
		//mReadPos = (mReadPos + len) % mBufferSize;
	}

	return len;
}

uint32 RecvRingBuffer::Peek(BYTE* dest, const uint32 len)
{
	if (GetUsedSize() < len)
	{
		return 0;
	}

	const uint32 PrevReadPos = GetReadPos();
	const bool IsOverBuffer = (PrevReadPos + len) > GetTotalSize() ? true : false;
	if (IsOverBuffer)
	{
		const uint32 OverLen = (PrevReadPos + len) - GetTotalSize();
		const uint32 LessLen = len - OverLen;

		::memcpy(dest, &GetBuffer()[PrevReadPos], LessLen);
		::memcpy(&dest[LessLen], GetBuffer(), OverLen);
	}
	else
	{
		::memcpy(dest, &GetBuffer()[PrevReadPos], len);
	}

	return len;
}
