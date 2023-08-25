#pragma once

#ifdef DLL_DEVELOPMENT
#define APEIROGON_API __declspec(dllimport)
#else
#define APEIROGON_API __declspec(dllexport)
#endif // DLL_DEVELOPMENT

#define _CRT_SECURE_NO_WARNINGS

//==========================//
//		   Default			//
//==========================//
#include <tchar.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <memory>
#include <chrono>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <locale>
#include <cstdlib>
#include <algorithm>
#include <stdarg.h>
#include <ctime>
#include <sys/timeb.h>
#include <typeinfo>
#include <type_traits>
#include <random>

//==========================//
//		     STL			//
//==========================//
#include <array>
#include <unordered_set>
#include <set>
#include <vector>
#include <functional>
#include <queue>
#include <unordered_map>
#include <map>
#include <string>

//==========================//
//		   WinSock			//
//==========================//
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

//==========================//
//		     TYPE			//
//==========================//
#include "Foward.h"
#include "DefaultTypes.h"
#include "DefaultSize.h"
#include "DefaultMacros.h"
#include "MathFoward.h"
#include "GameFrameworkFoward.h"

//==========================//
//		    sync			//
//==========================//
#include <future>
#include <thread>
#include <mutex>
#include <atomic>
#include "FastSpinLock.h"
#include "SRWLockGuard.h"
#include "LockController.h"

//==========================//
//		    UTILS			//
//==========================//
#include "RingBuffer.h"
#include "Singleton.h"
#include "Logger.h"
#include "WinDump.h"
#include "EngineAssert.h"
#include "SendBuffer.h"
#include "CircularQueue.h"
#include "PriorityQueue.h"
#include "SendQueue.h"

//==========================//
//		    TIME			//
//==========================//
#include "RoundTripTime.h"
#include "SimpleTime.h"
#include "TimeStamp.h"

//==========================//
//		   DataBase			//
//==========================//
#import "C:\Program Files (x86)\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")
#include "./../Builds/include/apeirogon/msado15.tlh"
#include <oledb.h>

//==========================//
//		     ADO			//
//==========================//
#include "ADOVariant.h"
#include "ADOUtils.h"
#include "ADOConnection.h"
#include "ADORecordset.h"
#include "ADOCommand.h"
#include "ADOEvent.h"
#include "ADOTask.h"
#include "DatabaseManager.h"


//==========================//
//		   SOCKETS			//
//==========================//
#include "SocketTypes.h"
#include "SocketUtils.h"
#include "IPAddress.h"
#include "Icmp.h"
#include "Socket.h"

//==========================//
//		    TASK			//
//==========================//
#include "Task.h"
#include "TaskManager.h"

//==========================//
//		    Data			//
//==========================//
#include "DataManager.h"

//==========================//
//		   SERVER			//
//==========================//
#include "ThreadManager.h"
#include "IOCPEvent.h"
#include "IOCP.h"
#include "Listener.h"
#include "SessionMonitoring.h"
#include "Session.h"
#include "PacketSession.h"
#include "SessionManager.h"
#include "Service.h"

//==========================//
//		     SMTP			//
//==========================//
#include "SMTP.h"

//==========================//
//		    Math			//
//==========================//
#include "MathLibrary.h"

//==========================//
//		 GameFrameWork		//
//==========================//
#include "GameFrameworkLibrary.h"

#include "KDTree.h"