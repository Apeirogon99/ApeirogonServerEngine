#pragma once

class IPAddress
{
public:
	SOCKET_API IPAddress();
	SOCKET_API IPAddress(const IPAddress& ipAddr);
	SOCKET_API IPAddress& operator=(const IPAddress& ipAddr);

	SOCKET_API ~IPAddress();

public:
	SOCKET_API void					SetIp(const in_addr& IPv4Addr);
	SOCKET_API void					SetIp(const in6_addr& IPv6Addr);
	SOCKET_API void					SetIp(const sockaddr_in& IPv4Addr);
	SOCKET_API void					SetIp(const sockaddr_in6& IPv6Addr);
	SOCKET_API void					SetIp(const sockaddr_storage& IpAddr);
	SOCKET_API void					SetIp(const WCHAR* ip, const uint16 port, const EProtocolType type);

	SOCKET_API void					SetAnyAddress();
	SOCKET_API void					SetBroadcastAddress();
	SOCKET_API void					SetLoopbackAddress();

	SOCKET_API void					SetPort(const uint16 port);

public:
	SOCKET_API std::wstring			GetIp();
	SOCKET_API uint16				GetPort() const;
	SOCKET_API sockaddr_storage		GetSockAddr() const;
	SOCKET_API int32				GetAddrSize() const;
	SOCKET_API EProtocolType		GetProtocolType() const;

public:
	SOCKET_API void					Clear();
	SOCKET_API bool					IsValid() const;
	SOCKET_API bool					CompareEndpoints(const IPAddress& addr);
	SOCKET_API std::wstring			ToString();

protected:
	void							SetDefaultAddressFamily();
	void							SetAddress(const IPAddress& ipAddr);

private:
	sockaddr_storage mAddr;	//IPv6까지 한다면 이걸로 해야함, 언리얼 참고
};