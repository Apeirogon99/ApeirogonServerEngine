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

FVector FVector::operator+(const FVector& inVector) const
{
	FVector vector;
	vector.SetX(this->mX + inVector.mX);
	vector.SetY(this->mY + inVector.mY);
	vector.SetZ(this->mZ + inVector.mZ);
	return vector;
}

FVector FVector::operator-(const FVector& inVector) const
{
	FVector vector;
	vector.SetX(this->mX - inVector.mX);
	vector.SetY(this->mY - inVector.mY);
	vector.SetZ(this->mZ - inVector.mZ);
	return vector;
}

FVector FVector::operator*(const FVector& inVector) const
{
	FVector vector;
	vector.SetX(this->mX * inVector.mX);
	vector.SetY(this->mY * inVector.mY);
	vector.SetZ(this->mZ * inVector.mZ);
	return vector;
}

FVector FVector::operator/(const FVector& inVector) const
{
	FVector vector;
	vector.SetX(this->mX / inVector.mX);
	vector.SetY(this->mY / inVector.mY);
	vector.SetZ(this->mZ / inVector.mZ);
	return vector;
}

FVector FVector::operator+(float inBias) const
{
	FVector vector;
	vector.SetX(this->mX + inBias);
	vector.SetY(this->mY + inBias);
	vector.SetZ(this->mZ + inBias);
	return vector;
}

FVector FVector::operator-(float inBias) const
{
	FVector vector;
	vector.SetX(this->mX - inBias);
	vector.SetY(this->mY - inBias);
	vector.SetZ(this->mZ - inBias);
	return vector;
}

FVector FVector::operator*(float inScale) const
{
	FVector vector;
	vector.SetX(this->mX * inScale);
	vector.SetY(this->mY * inScale);
	vector.SetZ(this->mZ * inScale);
	return vector;
}

FVector FVector::operator/(float inScale) const
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

std::ostream& operator<<(std::ostream& inOstream, const FVector& inVector)
{
	inOstream << "(X = " << inVector.mX << ", Y = " << inVector.mY << ", Z = " << inVector.mZ << ")";
	return inOstream;
}

bool FVector::Comapre(const FVector& inVector1, const FVector& inVector2)
{
	return inVector1.mX == inVector2.mX && inVector1.mY == inVector2.mY && inVector1.mZ == inVector2.mZ;
}

FRotator FVector::Rotator()
{
	FRotator rotator;
	rotator.SetYaw(std::atan2f(mY, mX) * (180.f / PI));
	rotator.SetPitch(std::atan2f(mZ, std::sqrtf(mX * mX + mY * mY)) * (180.f / PI));
	rotator.SetRoll(0.0f);
	return rotator;
}

float FVector::Length() const
{
	const float x = std::powf(GetX() , 2);
	const float y = std::powf(GetY() , 2);
	const float z = std::powf(GetZ() , 2);
	return std::sqrtf(x + y + z);
}

FVector FVector::Normalize() const
{
	FVector vector;
	float length = this->Length();
	vector.SetX(this->mX / length);
	vector.SetY(this->mY / length);
	vector.SetZ(this->mZ / length);
	return vector;
}

float FVector::Distance(const FVector& inVector1, const FVector& inVector2)
{
	bool compare = FVector::Comapre(inVector1, inVector2);
	if (compare)
	{
		return 0.0f;
	}

	const float x = std::powf(inVector1.GetX()- inVector2.GetX(), 2);
	const float y = std::powf(inVector1.GetY()- inVector2.GetY(), 2);
	const float z = std::powf(inVector1.GetZ()- inVector2.GetZ(), 2);
	return std::sqrtf(x + y + z);
}

float FVector::Distance2D(const FVector& inVector1, const FVector& inVector2)
{

	bool compare = FVector::Comapre(inVector1, inVector2);
	if (compare)
	{
		return 0.0f;
	}

	const float x = std::powf(inVector1.GetX() - inVector2.GetX(), 2);
	const float y = std::powf(inVector1.GetY() - inVector2.GetY(), 2);
	return std::sqrtf(x + y);
}

float FVector::DotProduct(const FVector& inVector1, const FVector& inVector2)
{
	return (inVector1.GetX() * inVector2.GetX()) + (inVector1.GetY() * inVector2.GetY()) + (inVector1.GetZ() * inVector2.GetZ());
}

FVector FVector::CrossProduct(const FVector& inVector1, const FVector& inVector2)
{
	FVector vector;
	vector.SetX(inVector1.GetY() * inVector2.GetZ() - inVector1.GetZ() * inVector2.GetY());
	vector.SetY(inVector1.GetZ() * inVector2.GetX() - inVector1.GetX() * inVector2.GetZ());
	vector.SetZ(inVector1.GetX() * inVector2.GetY() - inVector1.GetY() * inVector2.GetX());
	return vector;
}

FVector FVector::Projection(const FVector& inVector1, const FVector& inVector2)
{
	FVector vector;
	float dot	 = FVector::DotProduct(inVector1, inVector2);
	float length = inVector2.Length();
	vector = inVector2 * (dot / length);
	return vector;
}
