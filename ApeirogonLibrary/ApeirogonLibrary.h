#pragma once

#ifdef DLL_DEVELOPMENT
#define APEIROGON_API __declspec(dllimport)
#else
#define APEIROGON_API __declspec(dllexport)
#endif // DLL_DEVELOPMENT

#define _CRT_SECURE_NO_WARNINGS

#include "Generic.h"
#include "Database.h"
#include "Network.h"
#include "FileStorage.h"
//#include "Math.h"
//#include "Geometry.h"
//#include "Collision.h"


//==========================//
//		    Math			//
//==========================//
#include "MathLibrary.h"

//==========================//
//		 GameFrameWork		//
//==========================//
#include "GameFrameworkLibrary.h"

#include "KDTree.h"