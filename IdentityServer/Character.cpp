#include "pch.h"
#include "Character.h"

Character::Character()
{
}

Character::~Character()
{
}

void Character::SetAppearance(const Appearance& appearance)
{
	mAppearance = appearance;
}
