#pragma once
class Time
{
public:
	Time();
	~Time();

public:
	static std::wstring NowTime(const bool useMs);
	APEIROGON_API static int64 GetUTCTime();

};

