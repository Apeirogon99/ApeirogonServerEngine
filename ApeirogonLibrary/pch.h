#pragma once
#define WIN32_LEAN_AND_MEAN 

#ifdef DLL_DEVELOPMENT
#define APEIROGON_API __declspec(dllimport)
#else
#define APEIROGON_API __declspec(dllexport)
#endif // DLL_DEVELOPMENT

#include "ApeirogonLibrary.h"