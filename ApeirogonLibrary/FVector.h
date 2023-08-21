#pragma once

class APEIROGON_API FVector
{
public:
	FVector();
	FVector(const FVector& inVector);
	FVector(float InX, float InY, float InZ);
	FVector& operator=(const FVector& inVector);

public:
	FVector operator+(const FVector& inVector) const;
	FVector operator-(const FVector& inVector) const;
	FVector operator*(const FVector& inVector) const;
	FVector operator/(const FVector& inVector) const;

	FVector operator+(float inBias) const;
	FVector operator-(float inBias) const;
	FVector operator*(float inScale) const;
	FVector operator/(float inScale) const;

	bool operator==(const FVector& inVector);

	friend std::ostream& operator<<(std::ostream& inOstream, const FVector& inVector2);

public:


public:
	bool		Comapre(const FVector& inVector1, const FVector& inVector2, const float inDiff);
	FRotator	Rotator();
	float		Length() const;
	FVector		Normalize() const;

public:
	static float	Distance(const FVector& inVector1, const FVector& inVector2);
	static float	Distance2D(const FVector& inVector1, const FVector& inVector2);
	static float	DotProduct(const FVector& inVector1, const FVector& inVector2);
	static FVector	CrossProduct(const FVector& inVector1, const FVector& inVector2);
	static FVector  Projection(const FVector& inVector1, const FVector& inVector2);

public:
	void SetX(const float inX) { mX = inX; }
	void SetY(const float inY) { mY = inY; }
	void SetZ(const float inZ) { mZ = inZ; }

public:
	float GetX() const { return mX; }
	float GetY() const { return mY; }
	float GetZ() const { return mZ; }
	void ToString() const { printf("X=%3.3f Y=%3.3f Z=%3.3f\n", mX, mY, mZ); }

private:
	float mX;
	float mY;
	float mZ;
};