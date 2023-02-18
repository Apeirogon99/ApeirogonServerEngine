#pragma once
#pragma warning(disable : 4251)

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#pragma comment(lib, "ApeirogonBuilds\\Debug\\ApeirogonLibrary.lib")
#pragma comment(lib, "ApeirogonBuilds\\Debug\\CommonLibrary.lib")
#pragma comment(lib, "Protobuf\\Builds\\debug\\lib\\libprotobufd.lib")
#pragma comment(lib, "Protobuf\\Builds\\debug\\lib\\libprotobuf-lited.lib")
#pragma comment(lib, "Protobuf\\Builds\\debug\\lib\\libprotocd.lib")
#else
#pragma comment(lib, "ApeirogonBuilds\\Release\\ApeirogonLibrary.lib")
#pragma comment(lib, "ApeirogonBuilds\\Release\\CommonLibrary.lib")
#pragma comment(lib, "Protobuf\\Builds\\release\\lib\\libprotobuf.lib")
#pragma comment(lib, "Protobuf\\Builds\\release\\lib\\libprotobuf-lite.lib")
#pragma comment(lib, "Protobuf\\Builds\\release\\lib\\libprotoc.lib")
#endif

#include "ApeirogonLibrary.h"
#include "CommonLibaray.h"
#include "IdentityLibaray.h"