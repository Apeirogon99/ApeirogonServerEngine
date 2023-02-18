#pragma once
class IdentityGameState : public SessionManager
{
public:
	IdentityGameState(const SessionFactory& sessionFactory, const uint32 maxSessionCount);
	virtual ~IdentityGameState();

	IdentityGameState(const IdentityGameState& player) = delete;
	IdentityGameState& operator=(const IdentityGameState& player) = delete;

public:


private:

};

