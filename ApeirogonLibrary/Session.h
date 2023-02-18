#pragma once

class Session : public IocpObject
{
	enum class Default
	{
		MAX_RECV_BUFFER = 0xffff,
		SESSION_IS_SENDING = 1,
		SESSION_IS_FREE = 0,
	};

public:
	APEIROGON_API Session();
	APEIROGON_API virtual ~Session();

private:
	Session(const Session& session) = delete;
	Session& operator=(const Session& session) = delete;

public:
	APEIROGON_API virtual HANDLE GetHandle() override;
	APEIROGON_API virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

public:
	void SetSessionManager(const SessionManagerRef& service);
	void SetIpAddress(IPAddressPtr& ipAddr);
	void Shutdown();

public:
	bool RegisterConnect();
	bool RegisterDisconnect();
	void RegisterRecv();
	void RegisterSend();

	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(const uint32 numOfBytes);
	void ProcessSend(const uint32 numOfBytes);

public:
	APEIROGON_API void Connect();
	APEIROGON_API void Disconnect(const WCHAR* cause);
	APEIROGON_API void Send(SendBufferPtr message);
	APEIROGON_API void Recv();

	APEIROGON_API virtual void OnConnected() abstract;
	APEIROGON_API virtual uint32 OnRecv(RecvRingBuffer& buffer, uint32 len) abstract;
	APEIROGON_API virtual void OnSend(uint32 len) abstract;
	APEIROGON_API virtual void OnDisconnected() abstract;

	APEIROGON_API bool IsValid();

	APEIROGON_API void SessionLog(const WCHAR* log, ...);

public:
	bool IsConnected() const;
	SessionPtr GetSession();
	WinSocketPtr GetWinSocket() const;
	IPAddressPtr GetIpAddress() const;
	RecvRingBuffer& GetRecvBuffer();
	APEIROGON_API SessionManagerPtr GetSessionManager();

private:
	SessionManagerRef mSessionManager;
	WinSocketPtr mSocket;
	RecvRingBuffer mRecvBuffer;
	IPAddressPtr mIpAddr;

	ConnectEvent	mConnectEvnet;
	DisconnectEvent mDisconnectEvent;
	RecvEvent		mRecvEvent;
	SendEvent		mSendEvent;

	volatile LONG	mIsSending;
	SendQueue		mSendQueue;
	std::queue<BYTE*> mSendBuffers;

	std::atomic<bool> mIsConnect;
};

