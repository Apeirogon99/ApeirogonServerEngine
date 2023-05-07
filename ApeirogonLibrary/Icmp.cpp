#include "pch.h"
#include "Icmp.h"

Icmp::Icmp() : mDLL(NULL), mIcmpFile(NULL)
{
	LoadICMP();
}

Icmp::~Icmp()
{
	FreeICMP();
}

bool Icmp::LoadICMP()
{
	//mDLL = LoadLibrary(L"ICMP.DLL");
	//if (mDLL == NULL)
	//{
	//	wprintf(L"[Icmp::LoadICMP] can't find ICMP.DLL\n");
	//	return false;
	//}

	mIcmpFile = IcmpCreateFile();
	if (mIcmpFile == INVALID_HANDLE_VALUE)
	{
		wprintf(L"[Icmp::LoadICMP] Unable to open handle.\n");
		return false;
	}

	memset(&mIpOptions, 0, sizeof(IP_OPTION_INFORMATION));
	mIpOptions.Ttl = 128;

	return true;
}

bool Icmp::FreeICMP()
{
	//if (mDLL != NULL)
	//{
	//	FreeLibrary(mDLL);
	//}

	if (mIcmpFile != INVALID_HANDLE_VALUE)
	{
		IcmpCloseHandle(mIcmpFile);
	}

	return true;
}

bool Icmp::SendEcho(IcmpEvent& inIcmpEvent)
{
	IPAddress* ipAddr = inIcmpEvent.mIpAddr.get();
	if (nullptr == ipAddr)
	{
		return false;
	}

	if (false == ipAddr->IsValid())
	{
		return false;
	}

	IN_ADDR addr;
	if (false == ipAddr->GetIp(addr))
	{
		return false;
	}
	

	DWORD result = 0;
	result = IcmpSendEcho2Ex
	(
		mIcmpFile, 
		inIcmpEvent.hEvent,
		NULL,
		NULL,
		NULL,
		addr.S_un.S_addr, 
		NULL, 
		NULL, 
		&mIpOptions, 
		reinterpret_cast<void*>(inIcmpEvent.mReplyBuffer.GetWriteBuffer()), 
		inIcmpEvent.mReplyBuffer.GetRecvMaxSize(), 
		inIcmpEvent.mTimeout
	);

	WaitForSingleObject(inIcmpEvent.hEvent, INFINITE);

	if (result == 0)
	{
		int32 code = GetLastError();
		if (code == ERROR_IO_PENDING)
		{
			return true;
		}
	}

	return false;
}