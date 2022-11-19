#pragma once
#include <iostream>
#include <string>

class Credentials
{
public:
	Credentials()
	{

	}

	Credentials(const std::wstring InId, const std::wstring InToekn) :
		Id(InId),
		Token(InToekn)
	{

	}


public:






public:

	std::wstring Id;

	std::wstring Token;
};