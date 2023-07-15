#pragma once

#define NONE_ANIMATION 0

using Location	= FVector;
using Rotation	= FRotator;
using Scale		= FVector;
using Velocity	= FVector;


class CollisionComponent;
class BoxCollisionComponent;
class CapsuleCollisionComponent;
class SphereCollisionComponent;



class GameObject;
using GameObjectPtr		= std::shared_ptr<GameObject>;
using GameObjectRef		= std::weak_ptr<GameObject>;

class World;
using WorldPtr			= std::shared_ptr<World>;
using WorldRef			= std::weak_ptr<World>;

class Actor;
using ActorPtr			= std::shared_ptr<Actor>;
using ActorRef			= std::weak_ptr<Actor>;

class Character;
using CharacterPtr		= std::shared_ptr<Character>;
using CharacterRef		= std::weak_ptr<Character>;

class RemoteClient;
using RemoteClientPtr	= std::shared_ptr<RemoteClient>;
using RemoteClientRef	= std::weak_ptr<RemoteClient>;

class RemotePlayer;
using RemotePlayerPtr	= std::shared_ptr<RemotePlayer>;
using RemotePlayerRef	= std::weak_ptr<RemotePlayer>;

//
using PlayerViewer		= std::set<RemoteClientPtr>;
using PlayerMonitors	= std::set<RemotePlayerPtr>;
using ActorViewer		= std::set<ActorPtr>;
using ActorMonitors		= std::set<ActorPtr>;