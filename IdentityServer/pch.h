#pragma once

#define WIN32_LEAN_AND_MEAN

#include "ApeirogonLibrary.h"

#ifdef _DEBUG
#pragma comment(lib, "ApeirgonBuilds\\Debug\\ApeirogonLibrary.lib")
//#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ApeirgonBuilds\\Release\\ApeirogonLibrary.lib")
//#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif