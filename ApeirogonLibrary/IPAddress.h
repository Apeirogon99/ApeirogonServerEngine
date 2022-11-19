#pragma once

#ifdef DLL_DEVELOPMENT
#define SOCKET_API __declspec(dllimport)
#else
#define SOCKET_API __declspec(dllexport)
#endif // DLL_DEVELOPMENT

class SOCKET_API IPAddress
{
public:
	IPAddress();
	IPAddress(IN_ADDR addr);
	IPAddress(SOCKADDR_IN sockAddr);
	IPAddress(std::wstring ip, uint16 port);

	IPAddress(IPAddress& ipAddr);

	~IPAddress();

public:
	void			SetIp(IN_ADDR& addr);
	void			SetIp(const SOCKADDR_IN& sockAddr);
	void			SetIp(const std::wstring ip, const uint16 port);
	void			SetPort(uint16 port);
	void			SetAnyAddress();
	void			SetBroadcastAddress();
	void			SetLoopbackAddress();

public:
	std::wstring	GetIp();
	uint16			GetPort();
	SOCKADDR_IN&	GetSockAddr();
	int32			GetAddrSize() const;
	EProtocolType	GetProtocolType() const;

public:
	void			Clear();
	bool			IsValid() const;
	bool			CompareEndpoints(const IPAddress& addr);
	std::wstring	ToString();

private:
	SOCKADDR_IN mAddr;
	//sockaddr_storage mAddr;	//IPv6까지 한다면 이걸로 해야함, 언리얼 참고
};