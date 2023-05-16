#pragma once
class SendBuffer
{
public:
	APEIROGON_API SendBuffer() : mBuffer(nullptr), mAllocSize(0), mWriteSize(0) {}
	APEIROGON_API SendBuffer(const uint32 inAllocSize) : mAllocSize(inAllocSize), mWriteSize(0)
	{
		mBuffer = new BYTE[inAllocSize]();
		memset(mBuffer, NULL, inAllocSize);
	}
	APEIROGON_API SendBuffer(BYTE* buffer, uint32 allocSize) : mBuffer(buffer), mAllocSize(allocSize), mWriteSize(0)
	{

	}

	APEIROGON_API ~SendBuffer()
	{
		delete[] mBuffer;
		mBuffer = nullptr;
	}

	BYTE*		Buffer() { return mBuffer; }
	uint32		AllocSize() { return mAllocSize; }
	uint32		WriteSize() { return mWriteSize; }

private:
	BYTE*				mBuffer;
	uint32				mAllocSize = 0;
	uint32				mWriteSize = 0;
};

