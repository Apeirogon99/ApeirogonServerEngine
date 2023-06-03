#include "pch.h"
#include "IPAddress.h"

using namespace std;

IPAddress::IPAddress()
{
	Clear();
}

IPAddress::IPAddress(const IPAddress& ipAddr)
{
	SetAddress(ipAddr);
}

IPAddress& IPAddress::operator=(const IPAddress& ipAddr)
{
	SetAddress(ipAddr);
	return *this;
}

IPAddress::~IPAddress()
{
	//wprintf(L"IPAddress::~IPAddress() : Close ip address\n");
}

void IPAddress::SetIp(const in_addr& inIPv4Addr)
{
	Clear();
	sockaddr_in* IPv4Addr = reinterpret_cast<sockaddr_in*>(&mAddr);
	IPv4Addr->sin_family = AF_INET;
	IPv4Addr->sin_addr = inIPv4Addr;
}

void IPAddress::SetIp(const in6_addr& inIPv6Addr)
{
	Clear();
	sockaddr_in6* IPv6Addr = reinterpret_cast<sockaddr_in6*>(&mAddr);
	IPv6Addr->sin6_family = AF_INET6;
	IPv6Addr->sin6_addr = inIPv6Addr;
}

void IPAddress::SetIp(const sockaddr_in& inIPv4Addr)
{
	Clear();
	sockaddr_in* IPv4Addr = reinterpret_cast<sockaddr_in*>(&mAddr);
	IPv4Addr->sin_family = AF_INET;
	IPv4Addr->sin_addr = inIPv4Addr.sin_addr;
	SetPort(inIPv4Addr.sin_port);
}

void IPAddress::SetIp(const sockaddr_in6& inIPv6Addr)
{
	Clear();
	sockaddr_in6* IPv6Addr = reinterpret_cast<sockaddr_in6*>(&mAddr);
	IPv6Addr->sin6_family = AF_INET6;
	IPv6Addr->sin6_addr = inIPv6Addr.sin6_addr;
	SetPort(inIPv6Addr.sin6_port);
}

void IPAddress::SetIp(const sockaddr_storage& IpAddr)
{
	Clear();
	const EProtocolType currentType = static_cast<const EProtocolType>(IpAddr.ss_family);

	if (currentType == EProtocolType::IPv4)
	{
		const sockaddr_in* IPv4Addr = reinterpret_cast<const sockaddr_in*>(&IpAddr);
		SetIp(IPv4Addr->sin_addr);
		SetPort(IPv4Addr->sin_port);
	}
	else if (currentType == EProtocolType::IPv6)
	{
		const sockaddr_in6* IPv6Addr = reinterpret_cast<const sockaddr_in6*>(&IpAddr);
		SetIp(IPv6Addr->sin6_addr);
		SetPort(IPv6Addr->sin6_port);
		reinterpret_cast<sockaddr_in6*>(&mAddr)->sin6_scope_id = IPv6Addr->sin6_scope_id;
	}
	else
	{
		//LOG
	}
}

void IPAddress::SetIp(const WCHAR* ip, const uint16 port, const EProtocolType type)
{

	Clear();
	//const size_t ipAddrLen = wcslen(ip);
	//if (ipAddrLen != INET_ADDRSTRLEN && ipAddrLen != INET6_ADDRSTRLEN)
	//{
	//	return;
	//}

	//EProtocolType currentType = (ipAddrLen == INET_ADDRSTRLEN) ? EProtocolType::IPv4 : EProtocolType::IPv6;

	EProtocolType currentType = type;

	if (currentType == EProtocolType::IPv4)
	{
		sockaddr_in* IPv4Addr = reinterpret_cast<sockaddr_in*>(&mAddr);
		IPv4Addr->sin_family = AF_INET;
		::InetPtonW(AF_INET, ip, &IPv4Addr->sin_addr);
		SetPort(port);
	}
	else if (currentType == EProtocolType::IPv6)
	{
		sockaddr_in6* IPv6Addr = reinterpret_cast<sockaddr_in6*>(&mAddr);
		IPv6Addr->sin6_family = AF_INET6;
		::InetPtonW(AF_INET6, ip, &IPv6Addr->sin6_addr);
		SetPort(port);
	}
	else
	{
		//LOG
	}
}

void IPAddress::SetIp(const WCHAR* inDns, const uint16 inPort)
{
	Clear();

	const WCHAR* nodeName = inDns;

	WCHAR tempPort[6];
	memset(tempPort, 0, 6);
	swprintf_s(tempPort, L"%d", inPort);
	const WCHAR* serviceName = tempPort;

	ADDRINFOW hint = { 0, };
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;
	hint.ai_family = AF_UNSPEC;

	ADDRINFOW* result;
	int32 infoRet = ::GetAddrInfoW(nodeName, serviceName, &hint, &result);
	if (infoRet != 0)
	{
		wprintf(L"Failed get hoost by name [%ws][%ws]", serviceName, nodeName);
		FreeAddrInfoW(result);
		return;
	}

	WCHAR	ip[16];
	const WCHAR* error = ::InetNtopW(AF_INET, &result->ai_addr->sa_data[2], ip, 16);
	if (nullptr == error)
	{
		wprintf(L"Failed get hoost by name [%ws][%ws]", serviceName, nodeName);
		FreeAddrInfoW(result);
		return;
	}

	if (result->ai_family == AF_INET)
	{
		sockaddr_in* IPv4Addr = reinterpret_cast<sockaddr_in*>(&mAddr);
		IPv4Addr->sin_family = AF_INET;
		::InetPtonW(AF_INET, ip, &IPv4Addr->sin_addr);
		SetPort(inPort);
	}
	else if (result->ai_family == AF_INET6)
	{
		sockaddr_in6* IPv6Addr = reinterpret_cast<sockaddr_in6*>(&mAddr);
		IPv6Addr->sin6_family = AF_INET6;
		::InetPtonW(AF_INET6, ip, &IPv6Addr->sin6_addr);
		SetPort(inPort);
	}

	FreeAddrInfoW(result);
}

void IPAddress::SetAnyAddress()
{
	EProtocolType currentType = GetProtocolType();

	if (currentType == EProtocolType::IPv4)
	{
		SetIp(in4addr_any);
	}
	else if (currentType == EProtocolType::IPv6)
	{
		SetIp(in6addr_any);
	}
	else
	{
		SetDefaultAddressFamily();
		SetAnyAddress();
	}
}

void IPAddress::SetBroadcastAddress()
{
	EProtocolType currentType = GetProtocolType();

	if (currentType == EProtocolType::IPv4)
	{
		SetIp(in4addr_broadcast);
	}
	else if (currentType == EProtocolType::IPv6)
	{
		//SetIp();
	}
	else
	{
		SetDefaultAddressFamily();
		SetBroadcastAddress();
	}
}

void IPAddress::SetLoopbackAddress()
{
	EProtocolType currentType = GetProtocolType();

	if (currentType == EProtocolType::IPv4)
	{
		SetIp(in4addr_loopback);
	}
	else if (currentType == EProtocolType::IPv6)
	{
		SetIp(in6addr_loopback);
	}
	else
	{
		SetDefaultAddressFamily();
		SetLoopbackAddress();
	}
}

void IPAddress::SetPort(const uint16 port)
{
	EProtocolType currentType = GetProtocolType();

	if (currentType == EProtocolType::IPv4)
	{
		sockaddr_in* IPv4Addr = reinterpret_cast<sockaddr_in*>(&mAddr);
		IPv4Addr->sin_port = htons(port);
	}
	else if (currentType == EProtocolType::IPv6)
	{
		sockaddr_in6* IPv6Addr = reinterpret_cast<sockaddr_in6*>(&mAddr);
		IPv6Addr->sin6_port = htons(port);
	}
	else
	{
		SetDefaultAddressFamily();
		SetPort(port);
	}
}

bool IPAddress::GetIp(std::wstring& outIP)
{
	EProtocolType currentType = GetProtocolType();

	if (currentType == EProtocolType::IPv4)
	{
		WCHAR IPv4AddrBuffer[INET_ADDRSTRLEN];
		const sockaddr_in* IPv4Addr = reinterpret_cast<const sockaddr_in*>(&mAddr);
		::InetNtopW(AF_INET, &IPv4Addr->sin_addr, IPv4AddrBuffer, INET_ADDRSTRLEN);
		outIP = IPv4AddrBuffer;
		return true;
	}
	else if (currentType == EProtocolType::IPv6)
	{
		WCHAR IPv6AddrBuffer[INET6_ADDRSTRLEN];
		const sockaddr_in6* IPv6Addr = reinterpret_cast<const sockaddr_in6*>(&mAddr);
		::InetNtopW(AF_INET6, &IPv6Addr->sin6_addr, IPv6AddrBuffer, INET6_ADDRSTRLEN);
		outIP = IPv6AddrBuffer;
		return true;
	}

	return false;
}

bool IPAddress::GetIp(IN_ADDR& outIP)
{
	EProtocolType currentType = GetProtocolType();

	if (currentType == EProtocolType::IPv4)
	{
		const sockaddr_in* IPv4Addr = reinterpret_cast<const sockaddr_in*>(&mAddr);
		outIP = IPv4Addr->sin_addr;
		return true;
	}

	return false;
}

bool IPAddress::GetIp(IN6_ADDR& outIP)
{
	EProtocolType currentType = GetProtocolType();

	if (currentType == EProtocolType::IPv4)
	{
		const sockaddr_in6* IPv6Addr = reinterpret_cast<const sockaddr_in6*>(&mAddr);
		outIP = IPv6Addr->sin6_addr;
		return true;
	}

	return false;
}

uint16 IPAddress::GetPort() const
{
	EProtocolType currentType = GetProtocolType();

	if (currentType == EProtocolType::IPv4)
	{
		const uint16 IPv4Port = reinterpret_cast<const sockaddr_in*>(&mAddr)->sin_port;
		return ntohs(IPv4Port);
	}
	else if (currentType == EProtocolType::IPv6)
	{
		const uint16 IPv6Port = reinterpret_cast<const sockaddr_in6*>(&mAddr)->sin6_port;
		return ntohs(IPv6Port);
	}
	else
	{
		//LOG
	}

	return -1;
}

EProtocolType IPAddress::GetProtocolType() const
{

	USHORT currentType = static_cast<USHORT>(mAddr.ss_family);

	if (currentType == AF_INET)
	{
		return EProtocolType::IPv4;
	}
	else if (currentType == AF_INET6)
	{
		return EProtocolType::IPv6;
	}

	return EProtocolType::None;
}

sockaddr_storage IPAddress::GetSockAddr() const
{
	return mAddr;
}

int32 IPAddress::GetAddrSize() const
{
	EProtocolType currentType = GetProtocolType();

	switch (currentType)
	{
	case EProtocolType::None:
		return sizeof(sockaddr_storage);
		break;
	case EProtocolType::IPv4:
		return sizeof(SOCKADDR_IN);
		break;
	case EProtocolType::IPv6:
		return sizeof(SOCKADDR_IN6);
		break;
	default:
		break;
	}

	return 0;
}

void IPAddress::Clear()
{
	const size_t addrSize = GetAddrSize();
	::memset(&mAddr, 0, addrSize);
}

bool IPAddress::IsValid() const
{
	EProtocolType currentType = GetProtocolType();

	switch (currentType)
	{
	case EProtocolType::None:
		return false;
		break;
	case EProtocolType::IPv4:
		return reinterpret_cast<const sockaddr_in*>(&mAddr)->sin_addr.s_addr != 0;
		break;
	case EProtocolType::IPv6:
		return reinterpret_cast<const sockaddr_in6*>(&mAddr)->sin6_addr.s6_addr != 0;
		break;
	default:
		return false;
		break;
	}
}

bool IPAddress::CompareEndpoints(const IPAddress& addr)
{
	return false;
}

std::wstring IPAddress::ToString()
{
	WCHAR host[NI_MAXHOST];
	WCHAR serverice[NI_MAXSERV];

	GetNameInfoW((sockaddr*)&mAddr, GetAddrSize(), host, NI_MAXHOST, serverice, NI_MAXSERV, 0);

	WCHAR info[NI_MAXHOST + NI_MAXSERV];
	std::wstring ip;
	if (GetIp(ip))
	{
		swprintf_s(info, L"[IP = %ws:%ws][HOST = %ws]", ip.c_str(), serverice, host);
	}

	return std::wstring(info);
}

void IPAddress::SetDefaultAddressFamily()
{
	mAddr.ss_family = AF_INET;
}

void IPAddress::SetAddress(const IPAddress& ipAddr)
{
	sockaddr_storage sockAddr = ipAddr.GetSockAddr();
	EProtocolType currentType = ipAddr.GetProtocolType();
	if (currentType == EProtocolType::IPv4)
	{
		const sockaddr_in* IPv4Addr = reinterpret_cast<const sockaddr_in*>(&sockAddr);
		SetIp(IPv4Addr->sin_addr);
		SetPort(IPv4Addr->sin_port);
	}
	else if (currentType == EProtocolType::IPv6)
	{
		const sockaddr_in6* IPv6Addr = reinterpret_cast<const sockaddr_in6*>(&sockAddr);
		SetIp(IPv6Addr->sin6_addr);
		SetPort(IPv6Addr->sin6_port);
	}
	else
	{

	}
}
