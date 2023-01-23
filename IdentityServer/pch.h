#pragma once

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#pragma comment(lib, "ApeirogonBuilds\\Debug\\ApeirogonLibrary.lib")
#pragma comment(lib, "ProtobufBuilds\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ApeirogonBuilds\\Release\\ApeirogonLibrary.lib")
#pragma comment(lib, "ProtobufBuilds\\Release\\libprotobuf.lib")
#endif

#include "ApeirogonLibrary.h"
#include "IdentityLibaray.h"