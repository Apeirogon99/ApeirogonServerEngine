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
//		     TYPE			//
//==========================//
#include "Foward.h"
#include "DefaultTypes.h"
#include "DefaultSize.h"
#include "DefaultMacros.h"
#include "MathFoward.h"
#include "GameFrameworkFoward.h"

//==========================//
//		    SYNC			//
//==========================//
#include <future>
#include <thread>
#include <mutex>
#include <atomic>
#include "FastSpinLock.h"
#include "SRWLockGuard.h"
#include "LockController.h"

//==========================//
//		   GENERIC			//
//==========================//
#include "RingBuffer.h"
#include "Singleton.h"
#include "Logger.h"
#include "WinDump.h"
#include "EngineAssert.h"
#include "SendBuffer.h"
#include "Generic/CircularQueue.h"
#include "PriorityQueue.h"
#include "SendQueue.h"

//==========================//
//		    TIME			//
//==========================//
#include "RoundTripTime.h"
#include "SimpleTime.h"
#include "TimeStamp.h"

//==========================//
//		    TASK			//
//==========================//
#include "Task.h"
#include "TaskManager.h"
