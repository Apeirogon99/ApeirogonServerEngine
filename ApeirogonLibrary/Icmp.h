#pragma once

#include <iphlpapi.h>
#include <icmpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

class IcmpEvent;

class Icmp
{

public:
	Icmp();
	~Icmp();

private:
	Icmp(const Icmp&) = delete;
	Icmp(Icmp&&) = delete;

	Icmp& operator=(const Icmp&) = delete;
	Icmp& operator=(Icmp&&) = delete;

protected:
	bool LoadICMP();
	bool FreeICMP();

public:
	bool SendEcho(IcmpEvent& inIcmpEvent);

private:
	HINSTANCE				mDLL;
	HANDLE					mIcmpFile;

	IP_OPTION_INFORMATION	mIpOptions;
};

