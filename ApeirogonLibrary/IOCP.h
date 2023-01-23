#pragma once

class WinSocket;

enum class EIOCPMode
{
	Single,
	Multi,
};

class IocpObject : public std::enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};

class IOCPServer
{
public:
	APEIROGON_API IOCPServer();
	APEIROGON_API virtual ~IOCPServer();

private:
	IOCPServer(const IOCPServer& iocp) = delete;
	IOCPServer& operator=(const IOCPServer& iocp) = delete;

public:
	bool Prepare(const ServicePtr& service);
	void Shutdown();
	bool RegisterSocketToIOCP(const WinSocketPtr sock);
	APEIROGON_API bool WorkDispatch(uint32 timeoutMs = INFINITE);
	APEIROGON_API bool PostDispatch(const uint32 len, const ULONG_PTR key);

protected:
	void IOCPServerLog(const WCHAR* func);

private:
	HANDLE mIOCPHandle;
	ServicePtr mService;
};
