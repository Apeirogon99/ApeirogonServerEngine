#pragma once
class IdentityListener : public Listener
{
public:
	IdentityListener(IPAddressPtr& ipAddr);
	virtual ~IdentityListener();

	IdentityListener(const IdentityListener& player) = delete;
	IdentityListener& operator=(const IdentityListener& player) = delete;

};

