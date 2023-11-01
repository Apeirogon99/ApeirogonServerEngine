#include "pch.h"
#include "IOCPEvent.h"

IocpEvent::IocpEvent(EventType type) : eventType(type)
{
	Init();
}

void IocpEvent::Init()
{
	OVERLAPPED::hEvent = 0;
	OVERLAPPED::Internal = 0;
	OVERLAPPED::InternalHigh = 0;
	OVERLAPPED::Offset = 0;
	OVERLAPPED::OffsetHigh = 0;
}

void IocpEvent::SetOwner(IOCPObjectPtr inOwner)
{
	owner = inOwner;
}

void AcceptEvent::SetSession(SessionPtr inSession)
{
	session = inSession;
}
