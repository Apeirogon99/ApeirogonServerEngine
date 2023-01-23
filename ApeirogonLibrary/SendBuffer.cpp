#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer() : mBuffer(nullptr), mAllocSize(0), mWriteSize(0)
{
}

SendBuffer::SendBuffer(BYTE* buffer, uint32 allocSize) : mBuffer(buffer), mAllocSize(allocSize), mWriteSize(0)
{
}

SendBuffer::~SendBuffer()
{
	//wprintf(L"~SendBuffer() : %ld\n", mAllocSize);
}
