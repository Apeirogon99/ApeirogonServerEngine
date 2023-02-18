#pragma once

class IocpEvent;
class ConnectEvent;
class DisconnectEvent;
class AcceptEvent;
class RecvEvent;
class SendEvent;

class SMTP;

class Session;
class PacketSession;
class Service;
class IocpObject;
class IOCPServer;
class Listener;
class WinSocket;
class IPAddress;

class SendBuffer;

class SessionManager;
class ThreadManager;
class LoggerManager;

using ServicePtr = std::shared_ptr<Service>;
using PacketSessionPtr = std::shared_ptr<PacketSession>;
using IOCPServerPtr = std::shared_ptr<IOCPServer>;
using IOCPObjectPtr = std::shared_ptr<IocpObject>;
using SessionPtr = std::shared_ptr<Session>;
using ListenerPtr = std::shared_ptr<Listener>;
using WinSocketPtr = std::shared_ptr<WinSocket>;
using IPAddressPtr = std::shared_ptr<IPAddress>;
using SMTPPtr = std::shared_ptr<SMTP>;

using SendBufferPtr = std::shared_ptr<SendBuffer>;

using SessionManagerPtr = std::shared_ptr<SessionManager>;
using SessionManagerRef = std::weak_ptr<SessionManager>;
using ThreadManagerPtr = std::shared_ptr<ThreadManager>;
using LoggerManagerPtr = std::shared_ptr<LoggerManager>;

using SessionFactory = std::function<SessionPtr(void)>;