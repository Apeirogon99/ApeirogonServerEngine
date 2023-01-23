#pragma once

enum class ELogMode
{
	Console,
	File,
};

class LogWriter
{
public:
	LogWriter();
	virtual ~LogWriter();

	std::wstring GetNowTime(bool useMs);
	std::wstring GetNowThreadId();
	std::wstring GetLogMessage(const WCHAR* fmt, va_list args);

	virtual void Writer(const WCHAR* fmt, ...) abstract;
};

class FileWrite : public LogWriter
{
public:
	FileWrite();
	virtual ~FileWrite();

	void Init(const WCHAR* filePath, const WCHAR* fileName);

public:
	virtual void Writer(const WCHAR* fmt, ...) override;
	const WCHAR* mFilePath;
	const WCHAR* mFileName;
	std::wfstream mFileStream;
};

class ConsoleWriter : public LogWriter
{
public:
	ConsoleWriter();
	virtual ~ConsoleWriter();

public:
	virtual void Writer(const WCHAR* fmt, ...) override;

private:
	HANDLE		mStdOut;
};

class LoggerManager
{
public:
	APEIROGON_API LoggerManager(const WCHAR* name, const ELogMode mode);
	APEIROGON_API LoggerManager(const WCHAR* name, const WCHAR* filePath, const ELogMode mode);
	APEIROGON_API virtual ~LoggerManager();

	LoggerManager(const LoggerManager& loggerManager) = delete;
	LoggerManager& operator=(const LoggerManager& loggerManager) = delete;

public:
	bool		Prepare(const ServicePtr& service);
	void		Shutdown();

public:
	void		WriteLog(const WCHAR* fmt, ...);

private:
	const WCHAR*			mName;
	ELogMode				mLogMode;
	ServicePtr				mService;
	LogWriter*				mLogWriter;
};

//[날짜:시간:분:초][ThreadId] Class : log
//ex [2023-01-06:16:41:123][tid=4096] ListenerManger : can't create any session because max session is null