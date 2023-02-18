#pragma once
class SMTP
{
	enum
	{
		MAX_BUFFER = 0x200,
	};
public:
	APEIROGON_API SMTP();
	APEIROGON_API virtual ~SMTP();

	SMTP(const SMTP&) = delete;
	SMTP& operator=(const SMTP&) = delete;

protected:
	bool Prepare();
	void Shutdown();
	void SMTPLog(const int8* log, ...);
	bool SetSMTPAddr();
	std::string base64Encode(const std::string inCode);

protected:
	void EHOL();
	void STARTTLS();
	void AUTHLOGIN();
	void AUTHLOGIN_NAVER();
	void AUTHPLAIN();
	void MAIL();
	void RCPT(const std::string& inTo);
	void DATA();
	void SUBJECT(const std::string& inTo, const std::string& verified);
	void QUIT();
	void BYE();

protected:
	void Recv();

public:
	APEIROGON_API void SendVerified(const std::string& inTo, const std::string& verified);
	APEIROGON_API bool IsConnect() const { return mIsConnect; };

private:
	WinSocketPtr	mSMTPSocket;
	IPAddressPtr	mSMTPIpAddr;
	bool			mIsConnect;
	int8			mRecvBuffer[MAX_BUFFER];
	int8			mSendBuffer[MAX_BUFFER];
};