#pragma once
#include <random>

class Random
{
public:
	APEIROGON_API static const double	GetNormalDistribution(const double& inAverage, const double& inStandardDeviation);

	APEIROGON_API static const int32	GetIntUniformDistribution(const int32& inMin, const int32& inMax);
	APEIROGON_API static const double	GetRealUniformDistribution(const double& inMin, const double& inMax);

	APEIROGON_API static const FVector	GetRandomVectorInRange2D(const FVector& inVector, const float& inRange);
	APEIROGON_API static const FVector	GetRandomVectorInRange2D(const FVector& inVector, const float& inMin, const float& inMax);

	APEIROGON_API static const bool		GetRandomProbability(const double& inMin, const double& inMax, const double& inProbability);

private:
	static std::random_device	mRandomDevice;
	static std::mt19937			mMersenneTwister;
};