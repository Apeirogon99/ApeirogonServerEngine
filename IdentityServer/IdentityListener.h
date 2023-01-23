#pragma once
class IdentityListener : public Listener
{
public:
	IdentityListener(const IPAddressPtr& ipAddr);
	virtual ~IdentityListener();

	IdentityListener(const IdentityListener& player) = delete;
	IdentityListener& operator=(const IdentityListener& player) = delete;

};

