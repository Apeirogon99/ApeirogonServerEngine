#include "pch.h"
#include "IdentityGameState.h"

IdentityGameState::IdentityGameState(const SessionFactory& sessionFactory, const uint32 maxSessionCount) : SessionManager(sessionFactory, maxSessionCount)
{
}

IdentityGameState::~IdentityGameState()
{
}