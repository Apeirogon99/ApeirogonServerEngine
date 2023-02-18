#pragma once

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#pragma comment(lib, "ApeirogonBuilds\\Debug\\ApeirogonLibrary.lib")
#else
#pragma comment(lib, "ApeirogonBuilds\\Release\\ApeirogonLibrary.lib")
#endif

#include "ApeirogonLibrary.h"
#include "CommonLibaray.h"