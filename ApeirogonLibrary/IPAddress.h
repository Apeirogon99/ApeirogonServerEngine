#pragma once

class IPAddress
{
public:
	APEIROGON_API IPAddress();
	APEIROGON_API IPAddress(const IPAddress& ipAddr);
	APEIROGON_API IPAddress& operator=(const IPAddress& ipAddr);

	APEIROGON_API ~IPAddress();

public:
	APEIROGON_API void					SetIp(const in_addr& inIPv4Addr);
	APEIROGON_API void					SetIp(const in6_addr& inIPv6Addr);
	APEIROGON_API void					SetIp(const sockaddr_in& inIPv4Addr);
	APEIROGON_API void					SetIp(const sockaddr_in6& inIPv6Addr);
	APEIROGON_API void					SetIp(const sockaddr_storage& IpAddr);
	APEIROGON_API void					SetIp(const WCHAR* ip, const uint16 port, const EProtocolType type);

	APEIROGON_API void					SetAnyAddress();
	APEIROGON_API void					SetBroadcastAddress();
	APEIROGON_API void					SetLoopbackAddress();

	APEIROGON_API void					SetPort(const uint16 port);

public:
	APEIROGON_API std::wstring			GetIp();
	APEIROGON_API uint16				GetPort() const;
	APEIROGON_API sockaddr_storage		GetSockAddr() const;
	APEIROGON_API int32				GetAddrSize() const;
	APEIROGON_API EProtocolType		GetProtocolType() const;

public:
	APEIROGON_API void					Clear();
	APEIROGON_API bool					IsValid() const;
	APEIROGON_API bool					CompareEndpoints(const IPAddress& addr);
	APEIROGON_API std::wstring			ToString();

protected:
	void							SetDefaultAddressFamily();
	void							SetAddress(const IPAddress& ipAddr);

private:
	sockaddr_storage mAddr;	//IPv6까지 한다면 이걸로 해야함, 언리얼 참고
};