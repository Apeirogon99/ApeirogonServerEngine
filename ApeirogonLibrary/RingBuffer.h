#pragma once

class RingBuffer
{
public:
	APEIROGON_API RingBuffer() : mWritePos(0), mReadPos(0), mIndexMask(0), mCapcity(0), mBuffer(nullptr)
	{

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
	APEIROGON_API bool	InitBuffer(const uint32 inCapcity)
	{
		if (inCapcity > UINT16_MAX)
		{
			wprintf(L"[RingBuffer] It's too big to make\n");
			return false;
		}

		if (mBuffer != nullptr)
		{
			wprintf(L"[RingBuffer] It's already make\n");
			return false;
		}

		int32 newCapcity = 0;
		if (!((inCapcity & (inCapcity - 1)) == 0))
		{
			newCapcity = static_cast<int32>(pow(2, floor(log2(inCapcity)) + 1));
		}
		else
		{
			newCapcity = static_cast<int32>(inCapcity);
		}

		mCapcity = newCapcity;
		mIndexMask = newCapcity - 1;
		mBuffer = new BYTE[newCapcity]();
		Clean();

		return (mBuffer != nullptr) ? true : false;
	}

	APEIROGON_API int32 Enqueue(const BYTE* inData, const int32 inSize)
	{
		int32 processSize = 0;
		if (GetFreeSize() < inSize)
		{
			return processSize;
		}

		const int32 PrevWritePos = GetWritePos();
		const bool IsOverBuffer = (PrevWritePos + inSize) > GetTotalSize() ? true : false;
		if (IsOverBuffer)
		{
			const int32 OverLen = (PrevWritePos + inSize) - GetTotalSize();
			const int32 LessLen = inSize - OverLen;
			const int32 UsedLen = OverLen + LessLen;

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

	APEIROGON_API int32 Dequeue(BYTE* OutData, const int32 inLen)
	{
		int32 processSize = 0;
		if (GetUsedSize() < inLen)
		{
			return processSize;
		}

		const int32 PrevReadPos = GetReadPos();
		const bool IsOverBuffer = (PrevReadPos + inLen) > GetTotalSize() ? true : false;
		if (IsOverBuffer)
		{
			const int32 OverLen = (PrevReadPos + inLen) - GetTotalSize();
			const int32 LessLen = inLen - OverLen;
			const int32 UsedLen = OverLen + LessLen;

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

	APEIROGON_API int32 Peek(BYTE* OutData, const int32 inLen)
	{
		int32 processSize = 0;
		if (GetUsedSize() < inLen)
		{
			return 0;
		}

		const int32 PrevReadPos = GetReadPos();
		const bool IsOverBuffer = (PrevReadPos + inLen) > GetTotalSize() ? true : false;
		if (IsOverBuffer)
		{
			const int32 OverLen = (PrevReadPos + inLen) - GetTotalSize();
			const int32 LessLen = inLen - OverLen;
			const int32 UsedLen = OverLen + LessLen;

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
	APEIROGON_API void MoveRear(const int32 inLen)
	{
		mWritePos = (mWritePos + inLen) & mIndexMask;
	}

	APEIROGON_API void MoveFront(const int32 inLen)
	{
		mReadPos = (mReadPos + inLen) & mIndexMask;
	}

	inline int32 GetWritePos() const
	{
		return mWritePos;
	}

	inline int32 GetReadPos() const
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

	inline int32 GetUsedSize() const
	{
		int32 usedSize = 0;
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

	inline int32 GetTotalSize() const
	{
		return mCapcity;
	}

	inline int32 GetFreeSize()	const
	{
		int32 freeSize = GetTotalSize() - GetUsedSize();
		return freeSize;
	}

	inline int32 GetRecvMaxSize() const
	{
		int32 maxSize = GetTotalSize() - GetWritePos();
		return maxSize;
	}

private:
	int32			mWritePos;
	int32			mReadPos;
	int32			mCapcity;
	int32			mIndexMask;
	BYTE*			mBuffer;
};