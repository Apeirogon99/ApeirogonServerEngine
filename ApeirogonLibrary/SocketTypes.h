#pragma once

enum class EProtocolType : uint8
{
	None = 0,
	IPv4 = 2,
	IPv6 = 23
};

enum class ESocketType : uint8
{
	SOCKTYPE_Unknown,
	SOCKTYPE_Streaming = 1,
	SOCKTYPE_Datagram = 2
};