#pragma once

//==========================//
//		   SOCKET			//
//==========================//
class SMTP;
class WinSocket;
class IPAddress;

using WinSocketPtr			= std::shared_ptr<WinSocket>;
using IPAddressPtr			= std::shared_ptr<IPAddress>;
using SMTPPtr				= std::shared_ptr<SMTP>;

//==========================//
//		  IOCP EVENT		//
//==========================//
class IocpEvent;
class ConnectEvent;
class DisconnectEvent;
class AcceptEvent;
class RecvEvent;
class SendEvent;


//==========================//
//		   Framework		//
//==========================//
class Session;
class PacketSession;
class Service;
class IocpObject;
class IOCPServer;
class Listener;

using ServicePtr			= std::shared_ptr<Service>;
using PacketSessionPtr		= std::shared_ptr<PacketSession>;
using IOCPServerPtr			= std::shared_ptr<IOCPServer>;
using IOCPObjectPtr			= std::shared_ptr<IocpObject>;
using SessionPtr			= std::shared_ptr<Session>;
using SessionRef			= std::weak_ptr<Session>;
using ListenerPtr			= std::shared_ptr<Listener>;

using SessionFactory		= std::function<SessionPtr(void)>;

//==========================//
//		    Manager			//
//==========================//
class SessionManager;
class ThreadManager;
class LoggerManager;
class DatabaseManager;
class DataManager;
class TaskManager;

using SessionManagerPtr		= std::shared_ptr<SessionManager>;
using SessionManagerRef		= std::weak_ptr<SessionManager>;
using ThreadManagerPtr		= std::shared_ptr<ThreadManager>;
using LoggerManagerPtr		= std::shared_ptr<LoggerManager>;
using DatabaseManagerPtr	= std::shared_ptr<DatabaseManager>;
using DatabaseManagerRef	= std::weak_ptr<DatabaseManager>;
using DataManagerPtr		= std::shared_ptr<DataManager>;
using TaskManagerPtr		= std::shared_ptr<TaskManager>;

//==========================//
//		   Memory			//
//==========================//
class SendBuffer;
class TaskNode;
class TaskQueue;

using SendBufferPtr			= std::shared_ptr<SendBuffer>;
using TaskNodePtr			= std::shared_ptr<TaskNode>;
using TaskQueuePtr			= std::shared_ptr<TaskQueue>;

//==========================//
//		   DataBase			//
//==========================//
class ADOConnection;
class ADOCommand;
class ADORecordset;
class ADOAsyncTask;
class DatabaseTaskQueue;

using ADOAsyncTaskPtr		= std::shared_ptr<ADOAsyncTask>;
using DatabaseTaskQueuePtr	= std::shared_ptr<DatabaseTaskQueue>;

//==========================//
//		   Network			//
//==========================//
class GameObject;

using GameObjectPtr			= std::shared_ptr<GameObject>;


//==========================//
//		    Sync			//
//==========================//
class FastSpinLock;
class FastLockGuard;