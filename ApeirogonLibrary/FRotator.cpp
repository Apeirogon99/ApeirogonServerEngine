#include "pch.h"
#include "FRotator.h"

FRotator::FRotator() : mPitch(0.0f), mYaw(0.0f), mRoll(0.0f)
{
}

FRotator::FRotator(const FRotator& inRotator) : mPitch(inRotator.mPitch), mYaw(inRotator.mYaw), mRoll(inRotator.mRoll)
{
}

FRotator::FRotator(float InPitch, float InYaw, float InRoll) : mPitch(InPitch), mYaw(InYaw), mRoll(InRoll)
{
}

FRotator& FRotator::operator=(const FRotator& inRotator)
{
    mPitch  = inRotator.mPitch;
    mYaw    = inRotator.mYaw; 
    mRoll   = inRotator.mRoll;
    return *this;
}

FRotator& FRotator::operator+(const FRotator& inRotator)
{
    mPitch  += inRotator.mPitch;
    mYaw    += inRotator.mYaw;
    mRoll   += inRotator.mRoll;
    return *this;
}

FRotator& FRotator::operator-(const FRotator& inRotator)
{
    mPitch  -= inRotator.mPitch;
    mYaw    -= inRotator.mYaw;
    mRoll   -= inRotator.mRoll;
    return *this;
}

FRotator& FRotator::operator*(float inScale)
{
    mPitch  *= inScale;
    mYaw    *= inScale;
    mRoll   *= inScale;
    return *this;
}

bool FRotator::operator==(const FRotator& inRotator)
{
    return mPitch == inRotator.mPitch && mYaw == inRotator.mYaw && mRoll == inRotator.mRoll;
}

FVector FRotator::Vector()
{
    const float PitchNoWinding = std::fmod(mPitch, 360.0f);
    const float YawNoWinding = std::fmod(mYaw, 360.0f);

    //float CP, SP, CY, SY;
    //std::sin(&SP, &CP, PitchNoWinding * (PI / 180.f));
    //FMath::SinCos(&SY, &CY, YawNoWinding * (PI / 180.f));
    //FVector vector = FVector(CP * CY, CP * SY, SP);
    return FVector();
}
