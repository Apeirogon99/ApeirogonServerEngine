#pragma once

class RingBuffer
{
public:
	APEIROGON_API RingBuffer(const uint32 inCapcity) : mWritePos(0), mReadPos(0)
	{
		uint32 newCapcity = static_cast<uint32>(pow(2, floor(log2(inCapcity)) + 1));
		mCapcity = newCapcity;
		mIndexMask = newCapcity - 1;
		mBuffer = new BYTE[newCapcity]();
		Clean();
	}

	APEIROGON_API ~RingBuffer()
	{
		if (mBuffer)
		{
			Clean();
			delete[] mBuffer;
		}

		mBuffer = nullptr;
	}

	RingBuffer(const RingBuffer&) = delete;
	RingBuffer(RingBuffer&&) = delete;
	RingBuffer& operator=(const RingBuffer&) = delete;
	RingBuffer& operator=(RingBuffer&&) = delete;

public:
	APEIROGON_API uint32 Enqueue(const BYTE* inData, const uint32 inSize)
	{
		uint32 processSize = 0;
		if (GetFreeSize() < inSize)
		{
			return processSize;
		}

		const uint32 PrevWritePos = GetWritePos();
		const bool IsOverBuffer = (PrevWritePos + inSize) > GetTotalSize() ? true : false;
		if (IsOverBuffer)
		{
			const uint32 OverLen = (PrevWritePos + inSize) - GetTotalSize();
			const uint32 LessLen = inSize - OverLen;
			const uint32 UsedLen = OverLen + LessLen;

			::memcpy(&GetBuffer()[PrevWritePos], &inData[0], LessLen);
			::memcpy(&GetBuffer()[0], &inData[LessLen], OverLen);
			MoveRear(UsedLen);
			processSize += UsedLen;
		}
		else
		{
			::memcpy(&GetBuffer()[PrevWritePos], inData, inSize);
			MoveRear(inSize);
			processSize += inSize;
		}

		return processSize;
	}

	APEIROGON_API uint32 Dequeue(BYTE* OutData, const uint32 inLen)
	{
		uint32 processSize = 0;
		if (GetUsedSize() < inLen)
		{
			return processSize;
		}

		const uint32 PrevReadPos = GetReadPos();
		const bool IsOverBuffer = (PrevReadPos + inLen) > GetTotalSize() ? true : false;
		if (IsOverBuffer)
		{
			const uint32 OverLen = (PrevReadPos + inLen) - GetTotalSize();
			const uint32 LessLen = inLen - OverLen;
			const uint32 UsedLen = OverLen + LessLen;

			::memcpy(OutData, &GetBuffer()[PrevReadPos], LessLen);
			::memcpy(&OutData[LessLen], &GetBuffer()[0], OverLen);
			MoveFront(OverLen + LessLen);
			processSize += UsedLen;
		}
		else
		{
			::memcpy(OutData, &GetBuffer()[PrevReadPos], inLen);
			MoveFront(inLen);
			processSize += inLen;
		}

		return processSize;
	}

	APEIROGON_API uint32 Peek(BYTE* OutData, const uint32 inLen)
	{
		uint32 processSize = 0;
		if (GetUsedSize() < inLen)
		{
			return 0;
		}

		const uint32 PrevReadPos = GetReadPos();
		const bool IsOverBuffer = (PrevReadPos + inLen) > GetTotalSize() ? true : false;
		if (IsOverBuffer)
		{
			const uint32 OverLen = (PrevReadPos + inLen) - GetTotalSize();
			const uint32 LessLen = inLen - OverLen;
			const uint32 UsedLen = OverLen + LessLen;

			::memcpy(OutData, &GetBuffer()[PrevReadPos], LessLen);
			::memcpy(&OutData[LessLen], GetBuffer(), OverLen);
			processSize += UsedLen;
		}
		else
		{
			::memcpy(OutData, &GetBuffer()[PrevReadPos], inLen);
			processSize += inLen;
		}

		return processSize;
	}

	APEIROGON_API void Clean()
	{

		mWritePos = 0;
		mReadPos = 0;
		::memset(mBuffer, NULL, mCapcity);
	}

public:
	APEIROGON_API void MoveRear(const uint32 inLen)
	{
		mWritePos = (mWritePos + inLen) & mIndexMask;
	}

	APEIROGON_API void MoveFront(const uint32 inLen)
	{
		mReadPos = (mReadPos + inLen) & mIndexMask;
	}

	inline uint32 GetWritePos() const
	{
		return mWritePos;
	}

	inline uint32 GetReadPos() const
	{
		return mReadPos;
	}

	APEIROGON_API inline BYTE* GetBuffer()
	{
		return mBuffer;
	}

	APEIROGON_API inline BYTE* GetWriteBuffer()
	{
		return &mBuffer[mWritePos];
	}

	APEIROGON_API inline BYTE* GetReadBuffer()
	{
		return &mBuffer[mReadPos];
	}

	inline uint32 GetUsedSize() const
	{
		uint32 usedSize = 0;
		if (GetWritePos() == GetReadPos())
		{
			usedSize = 0;
		}
		else if (GetWritePos() > GetReadPos())
		{
			usedSize = GetWritePos() - GetReadPos();
		}
		else if (GetWritePos() < GetReadPos())
		{
			usedSize = GetTotalSize() - (GetReadPos() - GetWritePos());
		}

		return usedSize;
	}

	inline uint32 GetTotalSize() const
	{
		return mCapcity;
	}

	inline uint32 GetFreeSize()	const
	{
		uint32 freeSize = GetTotalSize() - GetUsedSize();
		return freeSize;
	}

	inline uint32 GetRecvMaxSize() const
	{
		uint32 maxSize = GetTotalSize() - GetWritePos();
		return maxSize;
	}

private:
	uint32			mWritePos;
	uint32			mReadPos;
	uint32			mCapcity;
	uint32			mIndexMask;
	BYTE*			mBuffer;
};