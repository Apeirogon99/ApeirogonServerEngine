#pragma once

class Character
{
public:
	Character();
	~Character();

	Character(const Character& appearance) = delete;
	Character& operator=(const Character& appearance) = delete;

public:
	void SetAppearance(const Appearance& appearance);

private:
	WCHAR*		mNickname;
	uint32		mLevel;
	Appearance	mAppearance;
};

