#pragma once

class APEIROGON_API Hit
{
public:
	Hit() {}
	~Hit() {}

public:
	void SetImpactLocation(const FVector& inImpactLocation);

public:
	const FVector&	GetImpactLocation();

public:
	FVector mImpactLocation;
};