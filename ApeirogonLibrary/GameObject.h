#pragma once
class GameObject : public TaskQueue
{
public:
	APEIROGON_API GameObject(const SessionManagerRef& inSessionManager) : mSessionManger(inSessionManager), mName(nullptr)
	{
		Initialization();
	}

	APEIROGON_API GameObject(const SessionManagerRef& inSessionManager, const WCHAR* inName) : mSessionManger(inSessionManager), mName(inName)
	{
		Initialization();
	}

	APEIROGON_API virtual ~GameObject()
	{
		Destroy();
	}

public:
	APEIROGON_API virtual void Initialization() 
	{

	};
	APEIROGON_API virtual void Destroy()
	{
		mSessionManger.reset();
		mName = nullptr;
	};
	APEIROGON_API virtual void Tick() abstract;
	APEIROGON_API virtual bool IsValid() abstract;

public:
	APEIROGON_API void SetGameObjetName(const WCHAR* inName)
	{
		mName = inName;
	}

protected:
	template <typename... Types>
	APEIROGON_API void GameObjectLog(const WCHAR* inLog, Types... inArgs)
	{
		if (mName == nullptr)
		{
			mName = L"NULL";
		}

		SessionManagerPtr sessionManager = mSessionManger.lock();
		if (sessionManager)
		{
			std::wstring tempLog;
			tempLog.append(L"[GameObject::");
			tempLog.append(mName);
			tempLog.append(L"] ");
			tempLog.append(inLog);

			sessionManager->SessionManagerLog(tempLog.c_str(), inArgs...);
		}
	}

	

protected:
	SessionManagerRef	mSessionManger;
	const WCHAR*		mName;
};

