#include "pch.h"
#include "Trace.h"

bool Trace::TraceCheack(const Collision& inCollision, Hit& outHit)
{
	bool result = HitCheack(inCollision, outHit);
	return result;
}

bool Trace::TraceCheack(const std::map<int64, Collision>& inCollisions, std::vector<Hit>& outHits)
{
	if (inCollisions.size() == 0)
	{
		return false;
	}

	std::map<int64, Collision>::const_iterator collision = inCollisions.begin();
	for (collision; collision != inCollisions.end(); collision++)
	{
		Hit resultHit;
		if (true == HitCheack(collision->second, resultHit))
		{
			resultHit.mObjectID = collision->first;
			outHits.push_back(resultHit);
		}
	}

	return outHits.size() != 0;
}

bool BoxTrace::HitCheack(const Collision& inCollision, Hit& outHit)
{
	const CollisionType& collisionType = inCollision.GetCollisionType();
	switch (collisionType)
	{
	case CollisionType::Collision_Box:
		outHit = BoxCheack(static_cast<const BoxCollision&>(inCollision));
		break;
	case CollisionType::Collision_Capsule:
		break;
	case CollisionType::Collision_Sphere:
		break;
	default:
		break;
	}

	return true;
}

Hit BoxTrace::BoxCheack(const BoxCollision& inCollision)
{
	return Hit();
}


bool CapsuleTrace::HitCheack(const Collision& inCollision, Hit& outHit)
{
	return true;
}

bool SphereTrace::HitCheack(const Collision& inCollision, Hit& outHit)
{
	return true;
}

bool FrustumTrace::HitCheack(const Collision& inCollision, Hit& outHit)
{
	return true;
}