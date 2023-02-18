#include "pch.h"
#include "Verified.h"
#include <random>

Verified::Verified()
{
	MakeVerified();
}

Verified::~Verified()
{
}

void Verified::MakeVerified()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> dis(0, MAX_RANDOM);

	for (int i = 0; i < MAX_VERIFIED; i++) {
		mVerified += std::to_string(dis(gen));
	}
}
