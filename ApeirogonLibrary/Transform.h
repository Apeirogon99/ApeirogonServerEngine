#pragma once

class APEIROGON_API Transform
{
public:
	Transform() : mLocation(), mRotation(), mScale() {}
	Transform(const Location& inLocation, const Rotation& inRotation, const Scale& inScale) : mLocation(inLocation), mRotation(inRotation), mScale(inScale) {}
	Transform(Location&& inLocation, Rotation&& inRotation, Scale&& inScale) : mLocation(inLocation), mRotation(inRotation), mScale(inScale) {}
	~Transform() {}

	Transform(const Transform& inTransform) { SetTransform(inTransform); }
	Transform(Transform&& inTransform) noexcept { SetTransform(inTransform); }

	Transform& operator=(const Transform& inTransform) { SetTransform(inTransform); return *this; }
	Transform& operator=(Transform&& inTransform) noexcept { SetTransform(inTransform); return *this; }

public:
	void			 SetTransform(const Transform& inTransform) { mLocation = inTransform.mLocation; mRotation = inTransform.mRotation; mScale = inTransform.mScale; }
	void			 SetLocation(const Location& inLocation)	{ mLocation = inLocation; }
	void			 SetRotation(const Rotation& inRotation)	{ mRotation = inRotation; }
	void			 SetScale(const Scale& inScale)				{ mScale = inScale; }

public:
	const Location&  GetLocation()	const { return mLocation; }
	const Rotation&  GetRotation()	const { return mRotation; }
	const Scale&	 GetScale()		const { return mScale; }

private:
	Location	mLocation;
	Rotation	mRotation;
	Scale		mScale;
};