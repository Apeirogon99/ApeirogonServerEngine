#include "pch.h"
#include "Logger.h"

LoggerManager::LoggerManager(const WCHAR* name, const ELogMode mode) : mName(name), mLogMode(mode)
{
	if (mode == ELogMode::File || mode == ELogMode::Both)
	{
		wprintf(L"[LoggerManager::LoggerManager()] Invalid mode, Try to set the mode again");
	}

	mLogWriter = new ConsoleWriter;

	mLogWriter->mMode = mode;
}

LoggerManager::LoggerManager(const WCHAR* name, const WCHAR* filePath, const ELogMode mode) : mName(name), mLogMode(mode)
{
	if (mode == ELogMode::Console)
	{
		wprintf(L"[LoggerManager::LoggerManager()] Invalid mode, Try to set the mode again");
	}

	mLogWriter = new FileWrite();


	FileWrite* fw = static_cast<FileWrite*>(mLogWriter);
	fw->Init(filePath, mName);

	mLogWriter->mMode = mode;

}

LoggerManager::~LoggerManager()
{
	wprintf(L"[LoggerManager::~LoggerManager()]\n");

	if (mLogWriter != nullptr)
	{
		delete mLogWriter;
	}
	mLogWriter = nullptr;
}

bool LoggerManager::Prepare(ServicePtr service)
{
	this->mService = service;
	if (nullptr == mService)
	{
		return false;
	}

	if (nullptr == mLogWriter)
	{
		return false;
	}

	if (nullptr == mName)
	{
		return false;
	}

	if(mLogMode == ELogMode::File)
	{
		FileWrite* fw = static_cast<FileWrite*>(mLogWriter);
		const WCHAR* filePath = fw->mFilePath;
		if (nullptr == filePath)
		{
			return false;
		}
	}

	WriteLog(L"[LoggerManager::Prepare()] Creating a log\n");

	return true;
}

void LoggerManager::Shutdown()
{
	WriteLog(L"[LoggerManager::Shutdown()] logger manager successfully shutdown\n");
	mService.reset();
}

void LoggerManager::WriteLog(const WCHAR* fmt, ...)
{
	if (fmt == nullptr)
		return;

	mLogWriter->Writer(fmt);
}

FileWrite::FileWrite()
{
	
}

FileWrite::~FileWrite()
{
	if (mFileStream.is_open())
	{
		mFileStream.close();
		mFileStream.clear();
	}

	if (nullptr == mFilePath || nullptr == mFileName)
	{
		return;
	}
	
	std::wstring oldFilePath = mFilePath;
	size_t findLog = oldFilePath.find(L".log");
	if (findLog == std::wstring::npos)
	{
		return;
	}

	std::wstring newFilePath;
	newFilePath += oldFilePath.substr(0, findLog);
	newFilePath += Time::NowTime(false);
	newFilePath += L".log";

	if (0 != _wrename(oldFilePath.c_str(), newFilePath.c_str()))
	{
		char errorBuffer[80];
		_strerror_s(errorBuffer, 80, NULL);
		printf("[FileWrite::~FileWrite()] [%d] : %s\n", errno, errorBuffer);
	}

	
}

void FileWrite::Init(const WCHAR* filePath, const WCHAR* fileName)
{
	mFilePath = filePath;
	mFileName = fileName;

	mFileStream.imbue(std::locale("korean"));

	mFileStream.open(mFilePath, std::ios::out | std::ios::trunc);
	if (mFileStream.fail())
	{
		wprintf(L"[FileWrite::Init()] : failed to file stream open");
		return;
	}
	
	printf("[FileWrite::Init()] : Start write %ws log - Location(%s)\n", mFileName, mFileStream.getloc()._C_str());
}

void FileWrite::Writer(const WCHAR* fmt, ...)
{
	if (fmt == nullptr)
		return;

	va_list args;
	va_start(args, fmt);
	std::wstring messageLog = GetLogMessage(fmt, args);
	va_end(args);

	if (mMode == ELogMode::Both)
	{
		wprintf(L"%ws", messageLog.c_str());
		fflush(stdout);
	}

	mFileStream.write(messageLog.c_str(), messageLog.size());
	mFileStream.flush();
}

ConsoleWriter::ConsoleWriter()
{
	mStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
}

ConsoleWriter::~ConsoleWriter()
{
}

void ConsoleWriter::Writer(const WCHAR* fmt, ...)
{
	if (fmt == nullptr)
		return;

	std::wstring messageLog;

	va_list args;
	va_start(args, fmt);
	messageLog += GetLogMessage(fmt, args);
	va_end(args);

	wprintf(L"%ws", messageLog.c_str());
	messageLog.clear();
	fflush(stdout);
}

LogWriter::LogWriter()
{
	setlocale(LC_ALL, "");
}

LogWriter::~LogWriter()
{
}

std::wstring LogWriter::GetNowThreadId()
{
	std::wstring threadStr;
	//std::thread::id threadId = std::this_thread::get_id();
	DWORD threadId = GetCurrentThreadId();

	threadStr += L"[";
	threadStr += std::to_wstring(threadId);
	threadStr += L"] ";

	return threadStr;
}

std::wstring LogWriter::GetLogMessage(const WCHAR* fmt, va_list args)
{
	std::wstring tempLog;
	tempLog += Time::NowTime(true);
	tempLog += GetNowThreadId();

	std::array<WCHAR, 1024> logStr;
	::vswprintf_s(logStr.data(), logStr.size(), fmt, args);

	tempLog += logStr.data();

	return tempLog;
}
