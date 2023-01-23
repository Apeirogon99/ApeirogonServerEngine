#pragma once

class IdentityService;
class IdentityListener;
class IdentityGameState;
class IdentityPlayerState;
class Character;


using IdentityServicePtr = std::shared_ptr<IdentityService>;
using IdentityListenerPtr = std::shared_ptr<IdentityListener>;
using GameStatePtr = std::shared_ptr<IdentityGameState>;
using PlayerStatePtr = std::shared_ptr<IdentityPlayerState>;
using CharacterPtr = std::shared_ptr<Character>;