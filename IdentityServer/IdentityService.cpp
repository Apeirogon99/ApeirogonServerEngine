#include "pch.h"
#include "IdentityService.h"

using namespace std;

IdentityService::IdentityService()
{
	setlocale(LC_ALL, "");
	SocketUtils::Init();
	IdentityServerPacketHandler::Init();
}

IdentityService::~IdentityService()
{

}

bool IdentityService::SettingService()
{
	//IOCPServer
	IOCPServerPtr IocpServer = std::make_shared<IOCPServer>();
	if (false == SetIOCPServer(IocpServer))
	{
		return false;
	}

	//ThreadManager
	const uint32 MaxThreadCount = 1;
	ThreadManagerPtr threadManager = std::make_shared<ThreadManager>(MaxThreadCount);
	if (false == SetThreadManager(threadManager))
	{
		return false;
	}

	//SessionManager
	const SessionFactory sessionFactory = std::make_shared<IdentityPlayerState>;
	const uint32 maxSessionCount = 1;
	GameStatePtr GameState = std::make_shared<IdentityGameState>(sessionFactory, maxSessionCount);
	SessionManagerPtr sessionManager = ::static_pointer_cast<SessionManager>(move(GameState));
	if (false == SetSessionManager(sessionManager))
	{
		return false;
	}

	//Listener
	IPAddressPtr IdentityIpAddress = std::make_shared<IPAddress>();
	IdentityIpAddress->SetLoopbackAddress();
	IdentityIpAddress->SetPort(9000);

	IdentityListenerPtr identityListener = std::make_shared<IdentityListener>(IdentityIpAddress);
	ListenerPtr listener = ::static_pointer_cast<Listener>(move(identityListener));
	if (false == SetListener(listener))
	{
		return false;
	}

	//Logger
	const WCHAR* LoggerName = L"IdentityServer";
	const WCHAR* filePath = L"P:\\ApeirogonEngine\\ApeirogonEngine\\ApeirogonLogs\\Identity\\IdentityServer.log";
	ELogMode LogMode = ELogMode::Console;
	LoggerManagerPtr Logger = std::make_shared<LoggerManager>(LoggerName, LogMode);
	if (false == SetLoggerManager(Logger))
	{
		return false;
	}

	return true;
}

void IdentityService::Tick(const float deltaTime)
{

}
