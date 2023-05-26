#pragma once

enum class EMonitoringResult
{
	MR_None						= 0x0,
	MR_Success					= 0x1,
	MR_Already_Monitoring,
	MR_Not_Enough_Time,
	MR_Not_Exist_Session,
};

class SessionMonitoring
{
	enum class Default
	{
		MONITOR_MAX_SYNC_TIME = 0x3E8,
		MONITOR_MAX_HEARTBEAT = 0x3E8,
		MONITOR_IS_CHECK = 1,
		MONITOR_IS_FREE = 0,
	};

public:
	SessionMonitoring();
	~SessionMonitoring();

private:
	SessionMonitoring(const SessionMonitoring&) = delete;
	SessionMonitoring(SessionMonitoring&&) = delete;

	SessionMonitoring& operator=(const SessionMonitoring&) = delete;
	SessionMonitoring& operator=(SessionMonitoring&&) = delete;

public:
	bool				SetOwnerSession(SessionRef& inSessionRef);
	EMonitoringResult	CheckSession();
	std::wstring		GetDescription();

protected:
	void MonitoringLog(const EMonitoringResult& inResult);
	bool CheckPing(EMonitoringResult& outResult);

private:
	SessionRef		mOwnerSession;
	volatile LONG	mIsCheck;
	TimeStamp		mMonitoringTimeStamp;
	std::wstring	mDescription;

	int64			mLastPingCount;
	int64			mLastHeartBeatCount;
};

