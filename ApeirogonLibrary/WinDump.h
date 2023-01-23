#pragma once
#include "minidumpapiset.h"

typedef BOOL(WINAPI* WRITERDUMP)
(
    _In_ HANDLE hProcess,
    _In_ DWORD ProcessId,
    _In_ HANDLE hFile,
    _In_ MINIDUMP_TYPE DumpType,
    _In_opt_ PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
    _In_opt_ PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
    _In_opt_ PMINIDUMP_CALLBACK_INFORMATION CallbackParam
);

class WinDump
{
public:
	WinDump();
	virtual ~WinDump();

	WinDump(const WinDump& lock) = delete;
	WinDump& operator=(const WinDump& lock) = delete;

public:
    APEIROGON_API static void Init();
	APEIROGON_API static LONG WINAPI ExceptionFilter(struct _EXCEPTION_POINTERS* execptionPointer);
};

