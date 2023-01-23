#pragma once

class ARGB
{
public:
	union
	{
		unsigned int      color;
		struct argb
		{
			unsigned char a;
			unsigned char r;
			unsigned char g;
			unsigned char b;
		};
	};
};

enum class EEquip : uint8
{
	Hair,
	Face,
	EyeBrow,
	Eye,
	Hat,
	Upper,
	Bottom,
	Shose
};

class AppearanceItem
{
public:
	uint8	mEquip;
	uint32	mCode;
	ARGB	mColor;
};

class Appearance
{
public:
	AppearanceItem mHair;
	AppearanceItem mFace;
	AppearanceItem mEyeBrow;
	AppearanceItem mEye;
	AppearanceItem mHat;
	AppearanceItem mUpper;
	AppearanceItem mBottom;
	AppearanceItem mShose;
};