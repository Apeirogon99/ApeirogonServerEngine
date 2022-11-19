#pragma once

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
#include <windows.h>
#include <locale>
#include <cstdlib>
#include "DefaultTypes.h"
#include "DefaultSize.h"

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
#include "IPAddress.h"
#include "Socket.h"
#include "SocketUtils.h"