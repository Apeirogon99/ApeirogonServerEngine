#include "pch.h"
#include "Ticket.h"

Ticket::Ticket()
{
	MakeTicket();
}

Ticket::~Ticket()
{
}

bool Ticket::MakeTicket()
{
	std::string ticket;
	GUID guid;
	size_t tcnt;
	int8 szlld[129] = { NULL, };
	WCHAR wszlld[129] = { NULL, };
	errno_t error;
	
	HRESULT result = CoCreateGuid(&guid);

	if (result != S_OK)
	{
		wprintf(L"GUID생성에 실패 하였습니다.\n");
		return false;
	}
			
	result = StringFromGUID2(guid, wszlld, 128);
	if (result == 0)
	{
		wprintf(L"GUID변환에 실패 하였습니다.\n");
		return false;
	}
		
	error = ::wcstombs_s(&tcnt, szlld, wszlld, 128);
	if (error != 0)
	{
		wprintf(L"GUID MBSC 변환에 실패 하였습니다.\n");
		return false;
	}
		
	ticket = szlld;
	ticket.erase(ticket.begin());
	ticket.erase(ticket.end() - 1);
		
	//wprintf(L"GUID 생성 완료.\n");

	mTicket = ticket;
	return true;
}

std::string Ticket::GetTicket()
{
	return mTicket;
}
