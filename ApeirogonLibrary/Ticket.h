#pragma once
class Ticket
{
public:
	APEIROGON_API Ticket();
	APEIROGON_API ~Ticket();

public:
	APEIROGON_API std::string GetTicket();

protected:
	bool MakeTicket();

private:
	std::string mTicket;
};

