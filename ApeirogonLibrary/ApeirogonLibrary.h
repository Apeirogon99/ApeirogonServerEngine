#pragma once

#ifdef DLL_DEVELOPMENT
#define ADO_API __declspec(dllimport)
#define SOCKET_API __declspec(dllimport)
#define APEIROGON_API __declspec(dllimport)
#else
#define ADO_API __declspec(dllexport)
#define SOCKET_API __declspec(dllexport)
#define APEIROGON_API __declspec(dllexport)
#endif // DLL_DEVELOPMENT

//==========================//
//		   Default			//
//==========================//
#include <tchar.h>
#include <iostream>
#include <stdio.h>
#include <string>
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

//==========================//
//		   WinSock			//
//==========================//
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

//==========================//
//		     STL			//
//==========================//
#include <array>
#include <set>
#include <vector>
#include <functional>
#include <queue>

//==========================//
//		     TYPE			//
//==========================//
#include "Foward.h"
#include "DefaultTypes.h"
#include "DefaultSize.h"

//==========================//
//		    TIME			//
//==========================//
#include "SimpleTime.h"
#include "Runtime.h"

//==========================//
//		    sync			//
//==========================//
#include <future>
#include <thread>
#include <mutex>
#include <atomic>
#include "FastSpinLock.h"
#include "SendQueue.h"
#include "RWLock.h"

//==========================//
//		    UTILS			//
//==========================//
#include "RingBuffer.h"
#include "Singleton.h"
#include "ThreadManager.h"
#include "Logger.h"
#include "WinDump.h"
#include "EngineAssert.h"
#include "SendBuffer.h"

//==========================//
//		   DataBase			//
//==========================//
#import "C:\Program Files (x86)\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")
#include "./x64/msado15.tlh"

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
#include "ADOService.h"


//==========================//
//		   SOCKETS			//
//==========================//
#include "SocketTypes.h"
#include "SocketUtils.h"
#include "IPAddress.h"
#include "Socket.h"

//==========================//
//		   SERVER			//
//==========================//
#include "IOCPEvent.h"
#include "IOCP.h"
#include "Listener.h"
#include "Session.h"
#include "PacketSession.h"
#include "SessionManager.h"
#include "Service.h"