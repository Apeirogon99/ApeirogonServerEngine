#pragma once

#undef  PI
#define PI 					(3.1415926535897932f)

class APEIROGON_API Math
{
public:
	inline float static ToRadian(float value)
	{
		return value * PI * 180.0f;
	}
};

class FVector;
class FRotator;
class Trace;
class Collision;