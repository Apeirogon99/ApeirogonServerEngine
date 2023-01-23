#pragma once

class IdentityDBHandler
{
public:
	static void Init()
	{
		HRESULT hResult = S_FALSE;

		hResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if (FAILED(hResult))
		{
			return;
		}
	}

	static void ShutDown()
	{
		CoUninitialize();
	}
};

