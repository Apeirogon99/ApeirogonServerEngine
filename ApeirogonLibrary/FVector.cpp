#include "pch.h"
#include "FVector.h"

FVector::FVector() : mX(0), mY(0), mZ(0)
{
}

FVector::FVector(const FVector& inVector) : mX(inVector.mX), mY(inVector.mY), mZ(inVector.mZ)
{
}

FVector::FVector(float InX, float InY, float InZ) : mX(InX), mY(InY), mZ(InZ)
{
}

FVector& FVector::operator=(const FVector& inVector)
{
	mX = inVector.mX;
	mY = inVector.mY; 
	mZ = inVector.mZ;
	return *this;
}

FVector FVector::operator+(const FVector& inVector)
{
	FVector vector;
	vector.SetX(this->mX + inVector.mX);
	vector.SetY(this->mY + inVector.mY);
	vector.SetZ(this->mZ + inVector.mZ);
	return vector;
}

FVector FVector::operator-(const FVector& inVector)
{
	FVector vector;
	vector.SetX(this->mX - inVector.mX);
	vector.SetY(this->mY - inVector.mY);
	vector.SetZ(this->mZ - inVector.mZ);
	return vector;
}

FVector FVector::operator*(const FVector& inVector)
{
	FVector vector;
	vector.SetX(this->mX * inVector.mX);
	vector.SetY(this->mY * inVector.mY);
	vector.SetZ(this->mZ * inVector.mZ);
	return vector;
}

FVector FVector::operator/(const FVector& inVector)
{
	FVector vector;
	vector.SetX(this->mX / inVector.mX);
	vector.SetY(this->mY / inVector.mY);
	vector.SetZ(this->mZ / inVector.mZ);
	return vector;
}

FVector FVector::operator+(float inBias)
{
	FVector vector;
	vector.SetX(this->mX + inBias);
	vector.SetY(this->mY + inBias);
	vector.SetZ(this->mZ + inBias);
	return vector;
}

FVector FVector::operator-(float inBias)
{
	FVector vector;
	vector.SetX(this->mX - inBias);
	vector.SetY(this->mY - inBias);
	vector.SetZ(this->mZ - inBias);
	return vector;
}

FVector FVector::operator*(float inScale)
{
	FVector vector;
	vector.SetX(this->mX * inScale);
	vector.SetY(this->mY * inScale);
	vector.SetZ(this->mZ * inScale);
	return vector;
}

FVector FVector::operator/(float inScale)
{
	FVector vector;
	vector.SetX(this->mX / inScale);
	vector.SetY(this->mY / inScale);
	vector.SetZ(this->mZ / inScale);
	return vector;
}

bool FVector::operator==(const FVector& inVector)
{
	return mX == inVector.mX && mY == inVector.mY && mZ == inVector.mZ;
}

bool FVector::Comapre(const FVector& inVector1, const FVector& inVector2, const float inDiff)
{
	return false;
}

FRotator FVector::Rotator()
{
	FRotator rotator;
	rotator.SetYaw(std::atan2f(mY, mX) * (180.f / PI));
	rotator.SetPitch(std::atan2f(mZ, std::sqrtf(mX * mX + mY * mY)) * (180.f / PI));
	rotator.SetRoll(0.0f);
	return rotator;
}

float FVector::Length()
{
	const float x = std::powf(GetX() , 2);
	const float y = std::powf(GetY() , 2);
	const float z = std::powf(GetZ() , 2);
	return std::sqrtf(x + y + z);
}

void FVector::Normalize()
{
	float length = this->Length();
	mX /= length;
	mY /= length;
	mZ /= length;
}

float FVector::Distance(const FVector& inVector1, const FVector& inVector2)
{
	const float x = std::powf(inVector1.GetX()- inVector2.GetX(), 2);
	const float y = std::powf(inVector1.GetY()- inVector2.GetY(), 2);
	const float z = std::powf(inVector1.GetZ()- inVector2.GetZ(), 2);
	return std::sqrtf(x + y + z);
}

float FVector::Distance2D(const FVector& inVector1, const FVector& inVector2)
{
	const float x = std::powf(inVector1.GetX() - inVector2.GetX(), 2);
	const float y = std::powf(inVector1.GetY() - inVector2.GetY(), 2);
	return std::sqrtf(x + y);
}
