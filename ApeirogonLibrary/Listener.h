#pragma once

class Listener : public IocpObject
{
public:
	APEIROGON_API Listener(IPAddressPtr& ipAddr);
	APEIROGON_API virtual ~Listener();

public:
	bool Prepare(const ServicePtr& service);
	void Shutdown();
	void CloseSocket();

public:
	APEIROGON_API virtual HANDLE GetHandle() sealed;
	APEIROGON_API virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) sealed;

private:
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

public:
	WinSocketPtr GetListenerSocket();
	IPAddressPtr GetIpAddress();

protected:
	void		ListenerLog(const WCHAR* log, ...);

protected:
	WinSocketPtr mListenSocket;
	std::vector<AcceptEvent*> mAcceptEvents;
	ServicePtr mService;
	IPAddressPtr mIpAddr;
};

