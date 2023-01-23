#pragma once
class IdentityService : public Service
{
public:
	IdentityService();
	virtual ~IdentityService();

	IdentityService(const IdentityService& service) = delete;
	IdentityService& operator=(const IdentityService& service) = delete;

protected:
	virtual bool		SettingService() override;
	virtual void		Tick(const float deltaTime) override;

private:

};

