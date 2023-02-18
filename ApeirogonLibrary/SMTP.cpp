#include "pch.h"
#include "SMTP.h"

SMTP::SMTP()
{
	ZeroMemory(mRecvBuffer, MAX_BUFFER);
	ZeroMemory(mSendBuffer, MAX_BUFFER);
	mIsConnect = Prepare();
}

SMTP::~SMTP()
{
	Shutdown();
}

bool SMTP::Prepare()
{

	mSMTPSocket = SocketUtils::CreateSocket(EProtocolType::IPv4, ESocketType::SOCKTYPE_Streaming);
	if (nullptr == mSMTPSocket)
	{
		return false;
	}

	if (false == SetSMTPAddr())
	{
		return false;
	}

	if (false == mSMTPSocket->Connect(mSMTPIpAddr))
	{
		return false;
	}

	return true;
}

void SMTP::Shutdown()
{
	if (false == mSMTPSocket->Close())
	{
		SMTPLog("Failed close SMTP socket");
		return;
	}

	printf("SMTP::Shutdown() : smtp successfully shutdown\n");
}

void SMTP::SMTPLog(const int8* log, ...)
{
#if _DEBUG
	if (log == nullptr)
		return;

	va_list args;
	va_start(args, log);
	::vprintf(log, args);
	va_end(args);

	fflush(stdout);
#endif
}

bool SMTP::SetSMTPAddr()
{
	const WCHAR* nodeName = L"localhost";
	const WCHAR* serviceName = L"0";
	ADDRINFOW hint = {0, };
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;
	hint.ai_family = AF_INET;

	ADDRINFOW* result;
	int32 infoRet = ::GetAddrInfoW(nodeName, serviceName, &hint, &result);
	if (infoRet != 0)
	{
		SMTPLog("Failed get hoost by name [%ws][%ws]", serviceName, nodeName);
		return false;
	}

	WCHAR	ip[16];
	uint16	port = 25;
	const WCHAR* error = ::InetNtopW(AF_INET, &result->ai_addr->sa_data[2], ip, 16);
	if (nullptr == error)
	{
		SMTPLog("Failed get hoost by name [%ws][%ws]", serviceName, nodeName);
		return false;
	}

#if _DEBUG
	SMTPLog("SMTP\n");
	wprintf(L"info : %s : %d\n\n", ip, port);
#endif

	mSMTPIpAddr = std::make_shared<IPAddress>();
	mSMTPIpAddr->SetIp(ip, port, EProtocolType::IPv4);

	FreeAddrInfoW(result);

	return mSMTPIpAddr->IsValid();
}

std::string SMTP::base64Encode(const std::string inCode)
{
	const std::string BASE64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string output;
	int value, value_b;

	value = 0; value_b = -6;
	for (unsigned char i : inCode) {
		value = (value << 8) + i;
		value_b += 8;

		while (value_b >= 0) {
			output.push_back(BASE64[(value >> value_b) & 0x3F]);
			value_b -= 6;
		}
	}

	if (value_b > -6) {
		output.push_back(BASE64[((value << 8) >> (value_b + 8)) & 0x3F]);
	}

	while (output.size() % 4) {
		output.push_back('=');
	}

	return output;
}

void SMTP::EHOL()
{

	int32 bytesSent = 0;

	Recv();

	// "google.co.kr";
	const int8* DNSAddr = "apeirogon";
	sprintf_s(mSendBuffer, MAX_BUFFER, "EHLO %s\r\n", DNSAddr);
	mSMTPSocket->Send(mSendBuffer, static_cast<int32>(strlen(mSendBuffer)), bytesSent);
	SMTPLog("c: %s", mSendBuffer);
}

void SMTP::STARTTLS()
{
	int32 bytesSent = 0;

	Recv();

	sprintf_s(mSendBuffer, MAX_BUFFER, "STARTTLS\r\n");
	mSMTPSocket->Send(mSendBuffer, static_cast<int32>(strlen(mSendBuffer)), bytesSent);
	SMTPLog("c: %s", mSendBuffer);

}

void SMTP::AUTHLOGIN()
{
	int32 bytesSent = 0;

	Recv();

	sprintf_s(mSendBuffer, MAX_BUFFER, "AUTH LOGIN\r\n");
	SMTPLog("c: %s", mSendBuffer);
	mSMTPSocket->Send(mSendBuffer, static_cast<int32>(strlen(mSendBuffer)), bytesSent);

	//USER NAME
	Recv();

	//
	const int8* USER_NAME = "apeirogon99";
	sprintf_s(mSendBuffer, MAX_BUFFER, "%s\r\n", base64Encode(USER_NAME).c_str());
	SMTPLog("c: %s", mSendBuffer);
	mSMTPSocket->Send(mSendBuffer, static_cast<int32>(strlen(mSendBuffer)), bytesSent);

	//USER PASSWORD
	Recv();
	
	//7CPVM8VJL9JU
	const int8* USER_PASSWORD = "hyngwhann@99";
	sprintf_s(mSendBuffer, MAX_BUFFER, "%s\r\n", base64Encode(USER_PASSWORD).c_str());
	SMTPLog("c: %s", mSendBuffer);
	mSMTPSocket->Send(mSendBuffer, static_cast<int32>(strlen(mSendBuffer)), bytesSent);

}

void SMTP::AUTHLOGIN_NAVER()
{
	int32 bytesSent = 0;

	Recv();

	sprintf_s(mSendBuffer, MAX_BUFFER, "AUTH LOGIN\r\n");
	SMTPLog("c: %s", mSendBuffer);
	mSMTPSocket->Send(mSendBuffer, static_cast<int32>(strlen(mSendBuffer)), bytesSent);

	//USER NAME
	Recv();

	//
	const int8* USER_NAME = "gwanho0218";
	sprintf_s(mSendBuffer, MAX_BUFFER, "%s\r\n", base64Encode(USER_NAME).c_str());
	SMTPLog("c: %s", mSendBuffer);
	mSMTPSocket->Send(mSendBuffer, static_cast<int32>(strlen(mSendBuffer)), bytesSent);

	//USER PASSWORD
	Recv();

	const int8* USER_PASSWORD = "lkyh#0218";
	sprintf_s(mSendBuffer, MAX_BUFFER, "%s\r\n", base64Encode(USER_PASSWORD).c_str());
	SMTPLog("c: %s", mSendBuffer);
	mSMTPSocket->Send(mSendBuffer, static_cast<int32>(strlen(mSendBuffer)), bytesSent);
}

void SMTP::AUTHPLAIN()
{
	int32 bytesSent = 0;

	Recv();

	const int8* PLAIN = "AGFwZWlyb2dvbjk5QGdtYWlsLmNvbQBoeml4cXB4d2Zlanp3d3Bk";
	sprintf_s(mSendBuffer, MAX_BUFFER, "AUTH PLAIN %s\r\n", PLAIN);
	SMTPLog("c: %s", mSendBuffer);
	mSMTPSocket->Send(mSendBuffer, static_cast<int32>(strlen(mSendBuffer)), bytesSent);
}

void SMTP::MAIL()
{

	int32 bytesSent = 0;

	Recv();

	const int8* From = "apeirogon99@localhost";
	sprintf_s(mSendBuffer, MAX_BUFFER, "mail from:<%s>\r\n", From);
	mSMTPSocket->Send(mSendBuffer, static_cast<int32>(strlen(mSendBuffer)), bytesSent);
	SMTPLog("c: %s", mSendBuffer);
}

void SMTP::RCPT(const std::string& inTo)
{
	int32 bytesSent = 0;

	Recv();

	const int8* To = inTo.c_str();
	sprintf_s(mSendBuffer, MAX_BUFFER, "rcpt to:<%s>\r\n", To);
	mSMTPSocket->Send(mSendBuffer, static_cast<int32>(strlen(mSendBuffer)), bytesSent);
	SMTPLog("c: %s", mSendBuffer);
}

void SMTP::DATA()
{
	int32 bytesSent = 0;

	Recv();

	sprintf_s(mSendBuffer, MAX_BUFFER, "data\r\n");
	mSMTPSocket->Send(mSendBuffer, static_cast<int32>(strlen(mSendBuffer)), bytesSent);
	SMTPLog("c: %s", mSendBuffer);
}

void SMTP::SUBJECT(const std::string& inTo, const std::string& verified)
{
	int32 bytesSent = 0;

	Recv();

	const int8* Subject = "Project_LD 이메일 인증";
	int8 MessageBuffer[MAX_BUFFER];
	sprintf_s(MessageBuffer, MAX_BUFFER, "이메일 인증 [ %s ]", verified.c_str());

	const int8* From = "apeirogon99@localhost";
	sprintf_s(mSendBuffer, MAX_BUFFER, "To:%s\nfrom:%s\nSubject:%s\r\n\r\n%s\r\n.\r\n", inTo.c_str(), From, Subject, MessageBuffer);
	mSMTPSocket->Send(mSendBuffer, static_cast<int32>(strlen(mSendBuffer)), bytesSent);
	SMTPLog("c: %s", mSendBuffer);
}

void SMTP::QUIT()
{
	int32 bytesSent = 0;

	Recv();

	sprintf_s(mSendBuffer, MAX_BUFFER, "quit\r\n");
	mSMTPSocket->Send(mSendBuffer, static_cast<int32>(strlen(mSendBuffer)), bytesSent);
	SMTPLog("c: %s", mSendBuffer);
}

void SMTP::BYE()
{
	int32 bytesSent = 0;

	Recv();
}

void SMTP::Recv()
{
	int32 bytesRead = 0;
	mSMTPSocket->Recv(mRecvBuffer, MAX_BUFFER, bytesRead);
	mRecvBuffer[bytesRead] = '\0';
	SMTPLog("s[%d]: %s", bytesRead, mRecvBuffer);
}

void SMTP::SendVerified(const std::string& inTo, const std::string& verified)
{
	EHOL();
	AUTHLOGIN();
	MAIL();
	RCPT(inTo);
	DATA();
	SUBJECT(inTo, verified);
	QUIT();
	BYE();
}
