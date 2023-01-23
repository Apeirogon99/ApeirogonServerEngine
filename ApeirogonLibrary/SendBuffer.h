#pragma once
class SendBuffer
{
public:
	APEIROGON_API SendBuffer();
	APEIROGON_API SendBuffer(BYTE* buffer, uint32 allocSize);
	APEIROGON_API ~SendBuffer();

	BYTE*		Buffer() { return mBuffer; }
	uint32		AllocSize() { return mAllocSize; }
	uint32		WriteSize() { return mWriteSize; }

private:
	BYTE*				mBuffer;
	uint32				mAllocSize = 0;
	uint32				mWriteSize = 0;
};

