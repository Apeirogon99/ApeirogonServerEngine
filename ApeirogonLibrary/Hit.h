#pragma once

class APEIROGON_API Hit
{
public:
	Hit() {}
	~Hit() {}

public:
	void SetObjectID(const int64 ObjectID);
	void SetImpactLocation(const FVector& inImpactLocation);

public:
	const int64		GetObjectID();
	const FVector&	GetImpactLocation();

public:
	int64	mObjectID;
	FVector mImpactLocation;
};