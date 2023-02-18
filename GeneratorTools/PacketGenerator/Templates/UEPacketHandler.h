#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Protobuf/Handler/FClientPacketHandler.h>

enum class EPakcetID: uint16
{
{%- for pkt in parser.total_pkt %}
	{{pkt.name}} = {{pkt.id}},
{%- endfor %}
};

// Custom Handlers
{%- for pkt in parser.recv_pkt %}
bool Handle_{{pkt.name}}(ANetworkController* controller, Protocol::{{pkt.name}}& pkt);
{%- endfor %}

class {{output}}
{
public:
	static void Init(PacketFunc inPacketFunc[UINT16_MAX])
	{
{%- for pkt in parser.recv_pkt %}
		inPacketFunc[static_cast<uint16>(EPakcetID::{{pkt.name}})] = [](ANetworkController* controller, BYTE* buffer, int32 len) { return FClientPacketHandler::HandlePacket<Protocol::{{pkt.name}}>(Handle_{{pkt.name}}, controller, buffer, len); };
{%- endfor %}
	}


{%- for pkt in parser.send_pkt %}
	static SendBufferPtr MakeSendBuffer(ANetworkController* controller, Protocol::{{pkt.name}}& pkt) { return FClientPacketHandler::MakeSendBuffer(controller, pkt, static_cast<uint16>(EPakcetID::{{pkt.name}})); }
{%- endfor %}

};
