#pragma once
class GameObject : public NetworkQueue
{
public:
	GameObject() : mName(nullptr)
	{

	}

	GameObject(const WCHAR* inName) : mName(inName)
	{
		Initialization();
	}

	virtual ~GameObject()
	{
		Destroy();
	}

public:
	virtual void Initialization() abstract;
	virtual void Destroy() abstract;

	void SetGameObjetName(const WCHAR* inName)
	{
		mName = inName;
	}

protected:
	void GameObjectLog(WCHAR* inLog, ...)
	{
		wprintf(L"[%ws] %ws", mName, inLog);
	}

protected:
	const WCHAR* mName;
};

