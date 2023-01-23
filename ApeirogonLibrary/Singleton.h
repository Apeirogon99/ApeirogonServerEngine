#pragma once

template<typename T>
class Singleton
{
protected:
	Singleton() = delete;
	~Singleton() = delete;

public:
	APEIROGON_API Singleton(const Singleton& singleton)
	{

	}

	APEIROGON_API Singleton& operator=(const Singleton& singleton)
	{

	}

	APEIROGON_API static T& GetInstance()
	{
		static T* instance = new T();
		return instance;
	}
};