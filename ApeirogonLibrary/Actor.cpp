#include "pch.h"
#include "Actor.h"

Actor::Actor(const WCHAR* inActorName) : GameObject(inActorName)
{
}

Actor::~Actor()
{
}

void Actor::CloseToActor(ActorPtr inCloseActor, float inPossibleVisbleLength)
{

	if (false == inCloseActor->IsValid())
	{
		return;
	}

	const float playerDistance = FVector::Distance(this->GetLocation(), inCloseActor->GetLocation());
	if (playerDistance < inPossibleVisbleLength)
	{
		OnAppearActor(inCloseActor);
	}
	else
	{
		OnDisAppearActor(inCloseActor);
	}
}

void Actor::SetWorld(WorldRef inWorld)
{
	mWorld = inWorld;
}

void Actor::SetTransform(const Transform& inTransform)
{
	mTransfrom.SetTransform(inTransform);
}

void Actor::SetTransform(const Location& inLocation, const Rotation& inRotation, const Scale& inScale)
{
	mTransfrom.SetTransform(Transform(inLocation, inRotation, inScale));
}

void Actor::SetLocation(const Location& inLocation)
{
	mTransfrom.SetLocation(inLocation);
}

void Actor::SetLocation(const float inX, const float inY, const float inZ)
{
	mTransfrom.SetLocation(Location(inX, inY, inZ));
}

void Actor::SetRotation(const Rotation& inRotator)
{
	mTransfrom.SetRotation(inRotator);
}

void Actor::SetRotation(const float inPitch, const float inYaw, const float inRoll)
{
	mTransfrom.SetRotation(Rotation(inPitch, inYaw, inRoll));
}

void Actor::SetScale(const Scale& inScale)
{
	mTransfrom.SetScale(inScale);
}

void Actor::SetScale(const float inX, const float inY, const float inZ)
{
	mTransfrom.SetScale(FVector(inX, inY, inZ));
}

void Actor::SetVelocity(const FVector& inVelocity)
{
	mVelocity = inVelocity;
}

void Actor::SetVelocity(const float inX, const float inY, const float inZ)
{
	mVelocity.SetX(inX);
	mVelocity.SetY(inY);
	mVelocity.SetZ(inZ);
}

bool Actor::FindPlayerViewer(RemoteClientPtr inRemoteClient)
{
	auto result = mPlayerViewers.find(inRemoteClient);
	return result != mPlayerViewers.end();
}

bool Actor::InsertPlayerViewer(RemoteClientPtr inRemoteClient)
{
	auto result = mPlayerViewers.insert(inRemoteClient);
	return result.second;
}

bool Actor::ReleasePlayerViewer(RemoteClientPtr inRemoteClient)
{
	size_t result = mPlayerViewers.erase(inRemoteClient);
	return result != 0;
}

void Actor::BrodcastPlayerViewers(SendBufferPtr inSendBuffer)
{
	for (auto viewer : mPlayerViewers)
	{
		viewer->Send(inSendBuffer);
	}
}
