#include "pch.h"
#include "IPAddress.h"

using namespace std;

IPAddress::IPAddress()
{
	Clear();
}

IPAddress::IPAddress(IN_ADDR addr)
{
	SetIp(addr);
}

IPAddress::IPAddress(SOCKADDR_IN sockAddr)
{
	SetIp(sockAddr);
}

IPAddress::IPAddress(std::wstring ip, uint16 port)
{
	SetIp(ip, port);
}

IPAddress::IPAddress(IPAddress& ipAddr)
{
	SetIp(ipAddr.GetSockAddr());
}

IPAddress::~IPAddress()
{
}

void IPAddress::SetIp(IN_ADDR& addr)
{
	Clear();
	((sockaddr_in*)&mAddr)->sin_addr = addr;
	mAddr.sin_family = AF_INET;
}

void IPAddress::SetIp(const SOCKADDR_IN& sockAddr)
{
	Clear();
	mAddr = sockAddr;
}

void IPAddress::SetIp(const std::wstring ip, const uint16 port)
{
	Clear();

	mAddr.sin_family = AF_INET;
	mAddr.sin_port = ::htons(port);
	::InetPtonW(AF_INET, ip.c_str(), &mAddr.sin_addr);
}

void IPAddress::SetPort(uint16 port)
{
	mAddr.sin_port = htons(port);
}

void IPAddress::SetAnyAddress()
{
	Clear();

	switch (GetProtocolType())
	{
	case EProtocolType::IPv4:
		((sockaddr_in*)&mAddr)->sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		mAddr.sin_family = AF_INET;
		break;
	case EProtocolType::IPv6:
		break;
	default:
		break;
	}
}

void IPAddress::SetBroadcastAddress()
{
	Clear();

	switch (GetProtocolType())
	{
	case EProtocolType::IPv4:
		((sockaddr_in*)&mAddr)->sin_addr.S_un.S_addr = htonl(INADDR_BROADCAST);
		mAddr.sin_family = AF_INET;
		break;
	case EProtocolType::IPv6:
		break;
	default:
		break;
	}
}

void IPAddress::SetLoopbackAddress()
{
	Clear();

	switch (GetProtocolType())
	{
	case EProtocolType::IPv4:
		((sockaddr_in*)&mAddr)->sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
		mAddr.sin_family = AF_INET;
		break;
	case EProtocolType::IPv6:
		break;
	default:
		break;
	}
}

std::wstring IPAddress::GetIp()
{
	const size_t IPv4Size = 16;
	WCHAR IPv4Buffer[IPv4Size];

	switch (mAddr.sin_family)
	{
	case AF_INET:
		::InetNtopW(AF_INET, &mAddr.sin_addr.S_un.S_addr, IPv4Buffer, IPv4Size);
		return ::wstring(IPv4Buffer);
		break;
	case AF_INET6:
		//WCHAR IPv6Buffer[46];
		//::InetNtopW(AF_INET6, &mAddr.sin_addr.S_un.S_addr, IPv6Buffer, sizeof(IPv6Buffer));
		//return ::wstring(IPv6Buffer);
		break;
	default:
		return L"";
		break;
	}

	return L"";
}

uint16 IPAddress::GetPort()
{
	return ::ntohs(mAddr.sin_port);
}

EProtocolType IPAddress::GetProtocolType() const
{
	switch (mAddr.sin_family)
	{
	case AF_INET:
		return EProtocolType::IPv4;
		break;
	case AF_INET6:
		return EProtocolType::IPv6;
		break;
	default:
		return EProtocolType::None;
		break;
	}
}

SOCKADDR_IN& IPAddress::GetSockAddr()
{
	return mAddr;
}

int32 IPAddress::GetAddrSize() const
{
	switch (GetProtocolType())
	{
	case EProtocolType::IPv4:
		return sizeof(SOCKADDR_IN);
		break;
	case EProtocolType::IPv6:
		return sizeof(SOCKADDR_IN6);
		break;
	default:
		return 0;
		break;
	}

	return 0;
}

void IPAddress::Clear()
{
	::memset(&mAddr, 0, GetAddrSize());
}

bool IPAddress::IsValid() const
{
	switch (GetProtocolType())
	{
	case EProtocolType::IPv4:
		return ((sockaddr_in*)&mAddr)->sin_addr.s_addr != 0;
		break;
	case EProtocolType::IPv6:
		return false;
		break;
	default:
		return false;
		break;
	}

	return false;
}

bool IPAddress::CompareEndpoints(const IPAddress& addr)
{
	return false;
}

std::wstring IPAddress::ToString()
{
	wstring Info;
	WCHAR host[NI_MAXHOST];
	WCHAR serverice[NI_MAXHOST];

	GetNameInfoW((sockaddr*)&mAddr, GetAddrSize(), host, NI_MAXHOST, serverice, NI_MAXHOST, 0);

	Info = GetIp() + L":" + wstring(serverice) + L"\t" + wstring(host);
	return Info;
}
