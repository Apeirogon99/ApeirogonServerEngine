#pragma once

//==========================//
//		   WinSock			//
//==========================//
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

//==========================//
//		   SOCKETS			//
//==========================//
#include "SocketTypes.h"
#include "SocketUtils.h"
#include "IPAddress.h"
#include "Icmp.h"
#include "Socket.h"

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