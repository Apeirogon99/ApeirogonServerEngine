#include "pch.h"
#include "SessionMonitoring.h"

SessionMonitoring::SessionMonitoring() : mIsCheck(static_cast<LONG>(Default::MONITOR_IS_FREE)),mMonitoringTimeStamp(L"SessionMonitoring"), mDescription(L"None"), mLastPingCount(0), mLastHeartBeatCount(0)
{
	
}

SessionMonitoring::~SessionMonitoring()
{
}

bool SessionMonitoring::SetOwnerSession(SessionRef& inSessionRef)
{
	mOwnerSession = inSessionRef;
	mMonitoringTimeStamp.StartTimeStamp();

	int64 stampTime = mMonitoringTimeStamp.GetTimeStamp();

	return (mOwnerSession.lock() != nullptr) ? true : false;
}

EMonitoringResult SessionMonitoring::CheckSession()
{
	EMonitoringResult result = EMonitoringResult::MR_Success;

	LONG IsSending = static_cast<LONG>(Default::MONITOR_IS_CHECK);
	IsSending = _InterlockedCompareExchange(&mIsCheck, static_cast<LONG>(Default::MONITOR_IS_CHECK), static_cast<LONG>(Default::MONITOR_IS_FREE));

	if (IsSending == static_cast<LONG>(Default::MONITOR_IS_CHECK))
	{
		return EMonitoringResult::MR_Already_Monitoring;
	}

	if (false == CheckPing(result))
	{
		MonitoringLog(result);
	}

	_InterlockedExchange(&mIsCheck, static_cast<LONG>(Default::MONITOR_IS_FREE));
	return result;
}

std::wstring SessionMonitoring::GetDescription()
{
	return mDescription;
}

void SessionMonitoring::MonitoringLog(const EMonitoringResult& inResult)
{
	switch (inResult)
	{
	case EMonitoringResult::MR_None:
		mDescription = L"None";
		break;
	case EMonitoringResult::MR_Success:
		mDescription = L"Success";
		break;
	case EMonitoringResult::MR_Not_Enough_Time:
		mDescription = L"Not enough time to execute";
		break;
	default:
		break;
	}
}

bool SessionMonitoring::CheckPing(EMonitoringResult& outResult)
{
	const int64 timeStamp = mMonitoringTimeStamp.GetTimeStamp();

	if (timeStamp < mLastPingCount + static_cast<int64>(Default::MONITOR_MAX_SYNC_TIME))
	{
		outResult = EMonitoringResult::MR_Not_Enough_Time;
		return true;
	}

	mLastPingCount = timeStamp;

	SessionPtr ownerSession = mOwnerSession.lock();
	if (ownerSession == nullptr)
	{
		outResult = EMonitoringResult::MR_Not_Exist_Session;
		return false;
	}

	ownerSession->RegisterIcmp();

	outResult = EMonitoringResult::MR_Success;
	return true;
}
