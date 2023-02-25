#pragma once

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

//==========================//
//		     STL			//
//==========================//
#include <array>
#include <set>
#include <vector>
#include <functional>
#include <queue>
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

//==========================//
//		     SMTP			//
//==========================//
#include "SMTP.h"