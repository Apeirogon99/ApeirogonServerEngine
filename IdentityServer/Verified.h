#pragma once
class Verified
{
	enum
	{
		MAX_RANDOM = 9,
		MAX_VERIFIED = 6,
	};

public:
	Verified();
	~Verified();

public:
	std::string GetVerified() const { return mVerified; };

protected:
	void MakeVerified();

private:
	std::string mVerified;
};

