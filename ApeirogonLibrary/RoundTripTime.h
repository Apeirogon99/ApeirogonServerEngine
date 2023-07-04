#pragma once
class RoundTripTime
{
	enum class Defualt
	{
		Capacity = 0x8,
	};

public:
	APEIROGON_API RoundTripTime() : mTimeStamps(static_cast<uint32>(Defualt::Capacity)), mFastSpinLock() {}
	APEIROGON_API ~RoundTripTime() {}

private:
	RoundTripTime(const RoundTripTime&) = delete;
	RoundTripTime(RoundTripTime&&) = delete;

	RoundTripTime& operator=(const RoundTripTime&) = delete;
	RoundTripTime& operator=(RoundTripTime&&) = delete;

public:
	APEIROGON_API bool AddLatency(const int64 inLatency)
	{
		FastLockGuard lockGuard(mFastSpinLock);

		if(mTimeStamps.IsFull())
		{
			mTimeStamps.Dequeue();
		}

		mTimeStamps.Enqueue(inLatency);
		return true;
	}

	APEIROGON_API bool AddLatency(const int64 inServiceTimeStamp, const int64 inClientTimeStamp)
	{
		const int64 latency = inServiceTimeStamp - inClientTimeStamp;
		if (latency < 0)
		{
			return false;
		}

		FastLockGuard lockGuard(mFastSpinLock);

		if (mTimeStamps.IsFull())
		{
			mTimeStamps.Dequeue();
		}

		mTimeStamps.Enqueue(latency);
		return true;
	}

	APEIROGON_API int64 GetRoundTripTime()
	{
		int64 roundTripTime = 0;
		int64 sumTime = 0;
		std::vector<int64> timestmaps;

		{
			FastLockGuard lockGuard(mFastSpinLock);
			mTimeStamps.Peek(timestmaps);
		}

		if (timestmaps.empty())
		{
			return 0;
		}

		for (const int64 time : timestmaps)
		{
			sumTime += time;
		}

		roundTripTime = static_cast<int64>(sumTime / timestmaps.size());

		return roundTripTime;
	}

	APEIROGON_API int64 GetRoundTripTimeHalf()
	{
		const int64 avgRTT = GetRoundTripTime();
		const int64 halfRTT = avgRTT / 2;
		return halfRTT;
	}

	APEIROGON_API int32 GetCapcity()
	{
		return mTimeStamps.GetCapcity();
	}

private:
	FastSpinLock			mFastSpinLock;
	CircularQueue<int64>	mTimeStamps;
};

