#pragma once
class APEIROGON_API FRotator
{
public:
	FRotator();
	FRotator(const FRotator& inRotator);
	FRotator(float InPitch, float InYaw, float InRoll);
	FRotator& operator=(const FRotator& inRotator);
	
public:
	FRotator& operator+(const FRotator& inRotator);
	FRotator& operator-(const FRotator& inRotator);
	FRotator& operator*(float inScale);

	bool operator==(const FRotator& inRotator);

public:
	static FRotator TurnRotator(const FRotator& inRotator);

public:
	FVector Vector() const;

public:
	void SetPitch(const float inPitch)	{ mPitch = inPitch; }
	void SetYaw(const float inYaw)		{ mYaw = inYaw; }
	void SetRoll(const float inRoll)	{ mRoll = inRoll; }

public:
	float GetPitch()	const { return mPitch; }
	float GetYaw()		const { return mYaw; }
	float GetRoll()		const { return mRoll; }

	FVector GetForwardVector() const;
	FVector GetRightVector() const;
	FVector GetUpVector() const;

	void ToString()		const { printf("Pitch=%3.3f Yaw=%3.3f Roll=%3.3f\n", mPitch, mYaw, mRoll); }

private:
	float mPitch;
	float mYaw;
	float mRoll;
};

