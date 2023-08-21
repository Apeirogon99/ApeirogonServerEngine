#include "pch.h"
#include "Random.h"

std::random_device Random::mRandomDevice;
std::mt19937 Random::mMersenneTwister = std::mt19937(mRandomDevice());

const double Random::GetNormalDistribution(const double& inAverage, const double& inStandardDeviation)
{
	std::normal_distribution<double> normalDist(inAverage, inStandardDeviation);
	return std::round(normalDist(mMersenneTwister));
}

const double Random::GetUniformDistribution(const double& inMin, const double& inMax)
{
	std::uniform_real_distribution<double> dis(inMin, inMax);
	return dis(mMersenneTwister);
}

const FVector Random::GetRandomVectorInRange2D(const FVector& inVector, const float& inRange)
{
	Location newVector;

	float minX = inVector.GetX() - inRange;
	float maxX = inVector.GetX() + inRange;

	float minY = inVector.GetY() - inRange;
	float maxY = inVector.GetY() + inRange;

	std::random_device randDevice;
	std::mt19937 gen(randDevice());
	std::uniform_real_distribution<> distX(minX, maxX);
	std::uniform_real_distribution<> distY(minY, maxY);

	newVector.SetX(static_cast<float>(distX(gen)));
	newVector.SetY(static_cast<float>(distY(gen)));
	newVector.SetZ(inVector.GetZ());
	return newVector;
}

const bool Random::GetRandomProbability(const double& inMin, const double& inMax, const double& inProbability)
{
	const double value = GetUniformDistribution(inMin, inMax);
	return (value <= inProbability) ? false : true;
}
