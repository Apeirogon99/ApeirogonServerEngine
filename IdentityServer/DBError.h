#pragma once

enum class ESPResult
{
	Success = 0,
	Failed = 1,
	Close = 2,
	Index_None,
};

class DBError
{
public:
	static std::wstring GetDBErrorToString(ESPResult result);
};