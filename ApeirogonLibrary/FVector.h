#pragma once
class APEIROGON_API FVector
{
public:
	FVector();
	FVector(const FVector& inVector);
	FVector(float InX, float InY, float InZ);
	FVector& operator=(const FVector& inVector);

public:
	FVector operator+(const FVector& inVector);
	FVector operator-(const FVector& inVector);
	FVector operator*(const FVector& inVector);
	FVector operator/(const FVector& inVector);

	FVector operator+(float inBias);
	FVector operator-(float inBias);
	FVector operator*(float inScale);
	FVector operator/(float inScale);

	bool operator==(const FVector& inVector);

public:
	FRotator	Rotator();
	float		Length();
	void		Normalize();

public:
	static float Distance(const FVector& inVector1, const FVector& inVector2);
	static float Distance2D(const FVector& inVector1, const FVector& inVector2);

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

