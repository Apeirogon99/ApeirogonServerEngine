#pragma once

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#pragma comment(lib, "ApeirgonBuilds\\Debug\\ApeirogonLibrary.lib")
#pragma comment(lib, "ProtobufBuilds\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ApeirgonBuilds\\Release\\ApeirogonLibrary.lib")
#pragma comment(lib, "ProtobufBuilds\\Release\\libprotobuf.lib")
#endif

#include "ApeirogonLibrary.h"