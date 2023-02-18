#include "pch.h"
#include "IdentityPacket.pb.h"
#include "OLEDB.h"

//WinSocket* sock = StartUp();
//
//	while (true)
//	{
//		DECODE_SINGIN(sock);
//
//		ENCODE_SINGING(sock);
//	}
//
//	UDP();
//
//	try
//	{
//
//		DatabaseInit();
//
//		Runtime runtime(L"NewConnection", ETime::second);
//		runtime.Start();
//		ADOConnection conn;
//		if (!conn.IsOpen())
//		{
//		}
//		runtime.End();
//
//		ADOVariant guidString(L"A7886D57-2A2B-4EB7-B082-C06C41B38496");
//
//		ADOCommand cmd;
//		cmd.SetStoredProcedure(conn, L"dbo.sp_test");
//		cmd.SetReturnParam();
//		cmd.SetInputParam(L"@input", guidString);
//
//		ADORecordset rs;
//		cmd.ExecuteStoredProcedure(rs, EExcuteReturnType::Wait_Return);
//
//		while (true)
//		{
//			if (rs.IsOpen())
//			{
//				printf("Execute Complete\n");
//				break;
//			}
//		}
//
//		int32 ret = cmd.GetReturnParam();
//		if (ret < 0)
//		{
//			printf("Error\n");
//		}
//		else
//		{
//			wprintf(L"RESULT START\n");
//			while (!(rs.IsEof())) {
//				int32 KEY = rs.GetFieldItem(L"TEST_KEY");
//				const WCHAR* OUTPUT = rs.GetFieldItem(L"TEST_OUTPUT");
//
//				auto tid = std::this_thread::get_id();
//				std::cout << "[tid : " << tid << "], ";
//				wprintf(L"[KEY  VALUES	: %d], ", KEY);
//				wprintf(L"[OUTPUT TYPE	: %ws]\n", OUTPUT);
//
//				rs.MoveNext();
//			}
//
//			wprintf(L"RESULT END\n");
//		}
//
//		rs.Close();
//		conn.Close();
//
//	}
//	catch (_com_error& e)
//	{
//		_bstr_t bstrSource(e.Source());
//		_bstr_t bstrDescription(e.Description());
//
//		wprintf(L"Error\n");
//		wprintf(L"\tCode = %08lx\n", e.Error());
//		wprintf(L"\tCode meaning = %s\n", e.ErrorMessage());
//		wprintf(L"\tSource = %s\n", bstrSource.GetBSTR());
//		wprintf(L"\tDescription = %s\n", bstrDescription.GetBSTR());
//	}
//
//g_conn.Close();
//struct PacketHeader
//{
//	uint16 packetType;
//	uint16 packetSize;
//};
//
//static int32 packetHeaderSize = static_cast<int32>(sizeof(PacketHeader));
//
//ADOConnection g_conn;
//
//WinSocket*  StartUp()
//{
//	wprintf(L"Identity Server\n");
//	WSADATA data;
//	WORD  ver = MAKEWORD(2, 2);
//
//	WSAStartup(ver, &data);
//
//	WinSocket* listening = SocketUtils::CreateSocket(EProtocolType::IPv4, ESocketType::SOCKTYPE_Datagram);
//
//	IPAddressPtr listeningAddr = std::make_shared<IPAddress>();
//	//listeningAddr->SetIp(L"127.0.0.1", 9000, EProtocolType::IPv4);
//	listeningAddr->SetLoopbackAddress();
//	listeningAddr->SetPort(9000);
//
//	std::wcout << "IP : " << listeningAddr->GetIp() << std::endl;
//	std::wcout << "PORT : " << listeningAddr->GetPort() << std::endl;
//
//	listening->Bind(listeningAddr);
//
//	//listening->Listen();
//
//	//WinSocket* clientSocket = listening->Accept();
//
//	//std::wcout << L"[CONNECT]" << clientSocket->GetAddress()->ToString() << std::endl;
//
//	//SocketUtils::DestroySocket(listening);
//
//	return listening;
//}
//
//void ENCODE_SINGING(WinSocket* sock)
//{
//
//	std::string idtoken;
//	GUID guid;
//	size_t tcnt;
//	int8 szlld[129] = { NULL, };
//	WCHAR wszlld[129] = { NULL, };
//	errno_t error;
//
//	HRESULT result = CoCreateGuid(&guid);
//
//	if (result != S_OK)
//	{
//		wprintf(L"GUID생성에 실패 하였습니다.\n");
//		idtoken = "";
//	}
//	
//	result = StringFromGUID2(guid, wszlld, 128);
//	if (result == 0)
//	{
//		wprintf(L"GUID변환에 실패 하였습니다.\n");
//		idtoken = "";
//	}
//
//	error = ::wcstombs_s(&tcnt, szlld, wszlld, 128);
//	if (error != 0)
//	{
//		wprintf(L"GUID MBSC 변환에 실패 하였습니다.\n");
//		idtoken = "";
//	}
//
//	idtoken = szlld;
//	idtoken.erase(idtoken.begin());
//	idtoken.erase(idtoken.end() - 1);
//
//	wprintf(L"GUID 생성 완료.\n");
//
//	Identity::S2C_Singin S2C_pktSingin;
//	S2C_pktSingin.set_error(0);
//	S2C_pktSingin.set_idtoken(idtoken.c_str());
//
//	const uint16 dataSize = static_cast<uint16>(S2C_pktSingin.ByteSizeLong());
//	const uint16 packetSize = dataSize + packetHeaderSize;
//
//	PacketHeader header;
//	header.packetType = C2S_SINGIN;
//	header.packetSize = packetSize;
//
//	char* sendBuf = new char[packetSize];
//	ZeroMemory(sendBuf, packetSize);
//
//	memcpy(sendBuf, &header, packetHeaderSize);
//	S2C_pktSingin.SerializeToArray(sendBuf + packetHeaderSize, dataSize);
//
//	int32 BytesSent = 0;
//	//sock->Send(sendBuf, (dataSize + packetHeaderSize), BytesSent);
//
//	IPAddressPtr ipAddr = std::make_shared<IPAddress>();
//	ipAddr->SetLoopbackAddress();
//	ipAddr->SetPort(9000);
//
//	sock->SendTo(sendBuf, (dataSize + packetHeaderSize), BytesSent, ipAddr);
//
//}
//
////void UpdateDB()
////{
////	int8 Recvbuf[4096];
////	int32 BytesRead = 0;
////
////	ZeroMemory(Recvbuf, 4096);
////	sock->Recv(Recvbuf, 4096, BytesRead);
////
////	if (BytesRead == 0)
////	{
////		printf("[DECODE_SINGIN] BytesRead is 0\n");
////		exit(0);
////	}
////
////	try
////	{
////		Runtime runtime(ETime::second);
////		runtime.Start();
////		DBConnection conn;
////		if (!conn.IsOpen())
////		{
////			conn.Open(connInfo, EDBMSTypes::MSSQL);
////		}
////		runtime.End();
////
////		const WCHAR* guidString = L"A7886D57-2A2B-4EB7-B082-C06C41B38496";
// 7E46A4BF-CCE4-49EA-BF7D-5FA35C9CEC80
////
////		DBCommand cmd;
////		cmd.SetStoredProcedure(conn, L"dbo.sp_test");
////		cmd.SetReturnParam();
////		cmd.SetInputParam(L"@input", guidString);
////
////		DBRecordset rs;
////		cmd.ExecuteStoredProcedure(rs, EExcuteReturnType::Async_Return);
////
////		while(true)
////		{
////			if (rs.IsOpen())
////			{
////				printf("Execute Complete\n");
////				break;
////			}
////		}
////
////		int32 ret = cmd.GetReturnParam();
////		if (ret < 0)
////		{
////			printf("Error\n");
////		}
////		else
////		{
////
////			while (!(rs.EndOfFile())) {
////				_variant_t KEY = rs.GetFieldItem(L"TEST_KEY");
////				_variant_t OUTPUT = rs.GetFieldItem(L"TEST_OUTPUT");
////
////				auto tid = std::this_thread::get_id();
////				std::cout << "[tid : " << tid << "], ";
////				wprintf(L"[KEY  VALUES	: %d], ", KEY.intVal);
////				wprintf(L"[OUTPUT TYPE	: %ld, %ws]\n", OUTPUT.vt, OUTPUT.bstrVal);
////
////				rs.MoveNext();
////			}
////
////		}
////
////		rs.Close();
////		conn.Close();
////
////	}
////	catch (_com_error& e)
////	{
////		_bstr_t bstrSource(e.Source());
////		_bstr_t bstrDescription(e.Description());
////
////		wprintf(L"Error\n");
////		wprintf(L"\tCode = %08lx\n", e.Error());
////		wprintf(L"\tCode meaning = %s\n", e.ErrorMessage());
////		wprintf(L"\tSource = %s\n", bstrSource.GetBSTR());
////		wprintf(L"\tDescription = %s\n", bstrDescription.GetBSTR());
////	}
////
////	UpdateDB();
////}
//
//void DECODE_SINGIN(WinSocket* sock)
//{
//	int8 Recvbuf[4096];
//	int32 BytesRead = 0;
//
//	ZeroMemory(Recvbuf, 4096);
//	//sock->Recv(Recvbuf, 4096, BytesRead);
//
//	IPAddressPtr ipAddr = std::make_shared<IPAddress>();
//	sock->RecvFrom(Recvbuf, 4096, BytesRead, ipAddr);
//
//	if (BytesRead == 0)
//	{
//		printf("[DECODE_SINGIN] BytesRead is 0\n");
//		exit(0);
//	}
//	else
//	{
//		std::wcout << L"[RecvFrom]" << ipAddr->ToString() << std::endl;
//	}
//
//	////패킷 받기
//	//PacketHeader headerRecv;
//	//memcpy(&headerRecv, Recvbuf, packetHeaderSize);
//
//	//Identity::C2S_Singin C2S_pktSingin;
//	//C2S_pktSingin.ParseFromArray(Recvbuf + packetHeaderSize, headerRecv.packetSize - packetHeaderSize);
//	//std::cout << "CLIENT > " << C2S_pktSingin.id() << " : " << C2S_pktSingin.password() << std::endl;
//
//	//try
//	//{
//	//	Runtime runtime(ETime::second);
//	//	runtime.Start();
//	//	DBConnection conn;
//	//	if (!conn.IsOpen())
//	//	{
//	//		conn.Open(connInfo.ToString(), EDBMSTypes::MSSQL);
//	//	}
//	//	runtime.End();
//
//	//	const char* id = C2S_pktSingin.id().c_str();
//	//	const char* password = C2S_pktSingin.password().c_str();
//
//	//	DBCommand cmd;
//	//	cmd.SetStoredProcedure(conn, L"dbo.singin");
//	//	cmd.SetReturnParam();
//	//	cmd.SetInputParam(L"@id", id);
//	//	cmd.SetInputParam(L"@password", password);
//
//	//	DBRecordset rs;
//	//	cmd.ExecuteStoredProcedure(rs, EExcuteReturnType::Wait_Return);
//
//	//	while (true)
//	//	{
//	//		switch (rs->GetState())
//	//		{
//	//		case adStateClosed:
//	//			printf("adStateClosed\n");
//	//			break;
//	//		case adStateOpen:
//	//			break;
//	//		case adStateConnecting:
//	//			printf("adStateConnecting\n");
//	//			break;
//	//		case adStateExecuting:
//	//			printf("adStateExecuting\n");
//	//			break;
//	//		case adStateFetching:
//	//			printf("adStateFetching\n");
//	//			break;
//	//		default:
//	//			printf("default\n");
//	//			break;
//	//		}
//
//	//		if (rs->GetState() == adStateOpen)
//	//		{
//	//			printf("adStateOpen\n");
//	//			break;
//	//		}
//	//	}
//
//	//	if (!rs.IsOpen())
//	//	{
//	//		wprintf(L"\nTRY OPEN\n");
//	//		rs.Open(cmd);
//	//	}
//	//	else
//	//	{
//	//		wprintf(L"RS RF : %ld", rs->GetRecordCount());
//	//		wprintf(L"\nAlready OPEN\n");
//	//	}
//
//	//	int32 ret = cmd.GetReturnParam();
//	//	if (ret < 0)
//	//	{
//	//		printf("Error\n");
//	//	}
//	//	else
//	//	{
//
//	//		while (!(rs.EndOfFile())) {
//	//			int32 gid				= rs.GetFieldItem(L"global_id").lVal;
//	//			const WCHAR* id			= rs.GetFieldItem(L"id").bstrVal;
//	//			const WCHAR* password	= rs.GetFieldItem(L"password").bstrVal;
//
//	//			std::cout << "GLOBAL_ID : " << gid << std::endl;
//	//			wprintf(L"ID		: %ws, %zd\n", id, wcslen(id));
//	//			wprintf(L"PASSWORD	: %ws, %zd\n", password, wcslen(password));
//
//	//			
//
//	//			rs.MoveNext();
//	//		}
//
//	//	}
//	//	
//	//	rs.Close();
//	//	conn.Close();
//
//	//}
//	//catch (_com_error& e)
//	//{
//	//	_bstr_t bstrSource(e.Source());
//	//	_bstr_t bstrDescription(e.Description());
//
//	//	wprintf(L"Error\n");
//	//	wprintf(L"\tCode = %08lx\n", e.Error());
//	//	wprintf(L"\tCode meaning = %s\n", e.ErrorMessage());
//	//	wprintf(L"\tSource = %s\n", bstrSource.GetBSTR());
//	//	wprintf(L"\tDescription = %s\n", bstrDescription.GetBSTR());
//	//}
//
//}
//
//
//ADOConnectionInfo TestConnectionInfo(L"SQLOLEDB", L"APEIROGON", L"TestDB", L"SSPI", L"NO", L"apeirogon", L"1248", EDBMSTypes::MSSQL);
//
//ADOService DBService;
//ADOConnection connectionIdentity;
//ADOConnection connectionTest;
//
//void UDP()
//{
//	wprintf(L"Identity Server\n");
//	WSADATA data;
//	WORD  ver = MAKEWORD(2, 2);
//
//	WSAStartup(ver, &data);
//
//	WinSocket* listening = SocketUtils::CreateSocket(EProtocolType::IPv4, ESocketType::SOCKTYPE_Datagram);
//
//	IPAddressPtr listeningAddr = std::make_shared<IPAddress>();
//	listeningAddr->SetLoopbackAddress();
//	listeningAddr->SetPort(9000);
//
//	listening->Bind(listeningAddr);
//
//	IPAddressPtr FromClient = std::make_shared<IPAddress>();
//	IPAddressPtr ToClient = std::make_shared<IPAddress>();
//
//	while (true)
//	{
//		int8 RecvBuf[4096];
//		ZeroMemory(RecvBuf, 4096);
//		int32 RecvSize = 0;
//
//		listening->RecvFrom(RecvBuf, 4096, RecvSize, FromClient);
//
//		if (RecvSize == 0)
//		{
//			printf("BytesRead is 0\n");
//			exit(0);
//		}
//
//		std::wcout << "RECV FROM CLIENT : " << FromClient->ToString() << std::endl;
//
//		//Sleep(1000);
//
//		int8 SendBuf[4096] = { "SEND TO SERVER\0" };
//		ZeroMemory(SendBuf, 4096);
//		int32 SendSize = 0;
//
//		const sockaddr_storage clientAddr = FromClient->GetSockAddr();
//		ToClient->SetIp(clientAddr);
//		listening->SendTo(SendBuf, 4096, SendSize, ToClient);
//
//		std::wcout << "SEND TO CLIENT : " << ToClient->ToString() << std::endl;
//
//	}
//
//	SocketUtils::DestroySocket(listening);
//}
//
//void DatabaseInit()
//{
//	Runtime runtime(L"DatabaseInit", ETime::second);
//	runtime.Start();
//
//	DBService.PushConnectionPool(connectionTest, TestConnectionInfo);
//	DBService.PrintConnectionPoolState();
//
//	runtime.End();
//}
//
//void RingbufferDebug(const RingBuffer& ringbuffer)
//{
//	std::wcout << " ----- RingbufferDebug -----" << std::endl;
//	std::wcout << "  WritePos : " << ringbuffer.GetWritePos() << std::endl;
//	std::wcout << "   ReadPos : " << ringbuffer.GetReadPos() << std::endl;
//	std::wcout << " Allocated : " << ringbuffer.GetAllocated() << std::endl;
//	std::wcout << "     Total : " << ringbuffer.GetTotal() << std::endl;
//	std::wcout << "      Free : " << ringbuffer.GetFree() << std::endl;
//	std::wcout << std::endl;
//}
//
//void Loot()
//{
//	Runtime time(L"AllocatedTime", ETime::ms);
//
//	time.Start();
//	RingBuffer ringbuffer;
//	time.End();
//
//
//	const BYTE enqueue[] = "I can hear you but I won't. Some look for trouble while others don't. There's a thousand reasons I should go about my da";
//
//	const uint32 enqueueSize = strlen(reinterpret_cast<const char*>(enqueue)) + 1;
//	BYTE* peek = new BYTE[enqueueSize];
//	BYTE* dequeue = new BYTE[enqueueSize];
//
//	RingbufferDebug(ringbuffer);
//
//	uint32 random = rand() % enqueueSize;
//	std::wcout << "Enqueue : " << ringbuffer.Enqueue2(enqueue, random) << std::endl << std::endl;
//
//
//	std::wcout << "Peek : ";
//	::memset(peek, NULL, enqueueSize);
//	std::wcout << ringbuffer.Peek(peek, random) << " : ";
//	std::wcout << reinterpret_cast<char*>(peek) << std::endl << std::endl;
//
//	std::wcout << "Dequeue : ";
//	::memset(dequeue, NULL, enqueueSize);
//	std::wcout << ringbuffer.Dequeue(dequeue, random) << " : ";
//	std::wcout << reinterpret_cast<char*>(dequeue) << std::endl << std::endl;
//}

using namespace std;

void StartServer()
{
	ADOConnectionInfo ConnectionInfo(L"SQLOLEDB", L"APEIROGON", L"account_database", L"SSPI", L"NO", L"apeirogon", L"1248", EDBMSTypes::MSSQL);
	ADOConnection conn;
	conn.Open(ConnectionInfo);

	IdentityServicePtr service = std::make_shared<IdentityService>();

	service->ServiceOpen();

	std::string userInput;
	while (true)
	{

		std::cin >> userInput;


		if (userInput == "q")
		{
			service->ServiceClose();
			break;
		}
	}

	cout << service.use_count() << endl;

	SocketUtils::Clear();
}

int main(void)
{
	StartServer();

	system("pause");
}