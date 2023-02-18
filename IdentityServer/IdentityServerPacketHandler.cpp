#include "pch.h"
#include "IdentityServerPacketHandler.h"
#include "IdentityDBHandler.h"
#include "Ticket.h"
#include "Verified.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionPtr& session, BYTE* buffer, int32 len)
{
	return false;
}

bool Handle_C2S_EnterServer(PacketSessionPtr& session, Protocol::C2S_EnterServer& pkt)
{
	return false;
}

bool Handle_C2S_LeaveServer(PacketSessionPtr& session, Protocol::C2S_LeaveServer& pkt)
{
	return false;
}

bool Handle_C2S_Singin(PacketSessionPtr& session, Protocol::C2S_Singin& pkt)
{
	bool valid = session->IsValid();
	if (false == valid)
	{
		return false;
	}

	PlayerStatePtr playerState = std::static_pointer_cast<IdentityPlayerState>(session);
	if (nullptr == playerState)
	{
		return false;
	}

	ADOVariant clientID = pkt.id().c_str();
	ADOVariant clientPS = pkt.password().c_str();

	ADOConnectionInfo ConnectionInfo(L"SQLOLEDB", L"APEIROGON", L"account_database", L"SSPI", L"NO", L"apeirogon", L"1248", EDBMSTypes::MSSQL);
	ADOConnection conn;
	conn.Open(ConnectionInfo);

	ADOCommand cmd;
	cmd.SetStoredProcedure(conn, L"dbo.singin_sp");
	cmd.SetReturnParam();
	cmd.SetInputParam(L"@id", clientID);
	cmd.SetInputParam(L"@password", clientPS);
	cmd.SetOutputParam(L"@global_id", DataTypeEnum::adInteger, sizeof(int32));
	cmd.SetOutputParam(L"@ticket", DataTypeEnum::adBSTR, 36);

	ADORecordset rs;
	cmd.ExecuteStoredProcedure(rs, EExcuteReturnType::Wait_Return);

	Protocol::S2C_Singin singinPacket;
	int32 ret = cmd.GetReturnParam();
	if (ret != 0)
	{
		singinPacket.set_error(ret);
		singinPacket.set_ticket("");

		//int32 global_id = cmd.GetOutputParam(L"@global_id");
		//playerState->SetGlobalID(global_id);
	}
	else
	{
		int32 global_id = cmd.GetOutputParam(L"@global_id");
		std::string ticket = cmd.GetOutputParam(L"@ticket");

		playerState->SetGlobalID(global_id);

		singinPacket.set_error(ret);
		singinPacket.set_ticket(ticket);
	}

	SendBufferPtr sendBuffer = IdentityServerPacketHandler::MakeSendBuffer(session, singinPacket);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C2S_Singup(PacketSessionPtr& session, Protocol::C2S_Singup& pkt)
{
	PlayerStatePtr playerState = std::static_pointer_cast<IdentityPlayerState>(session);
	if (nullptr == playerState)
	{
		return false;
	}

	std::string tempEmail = pkt.email();
	size_t find = tempEmail.find('@');

	ADOVariant clientID		= pkt.id().c_str();
	ADOVariant local		= tempEmail.substr(0, find).c_str();
	ADOVariant domain		= tempEmail.substr(find + 1, tempEmail.length()).c_str();
	ADOVariant clientPS		= pkt.password().c_str();

	ADOConnectionInfo ConnectionInfo(L"SQLOLEDB", L"APEIROGON", L"account_database", L"SSPI", L"NO", L"apeirogon", L"1248", EDBMSTypes::MSSQL);
	ADOConnection conn;
	conn.Open(ConnectionInfo);

	ADOCommand cmd;
	cmd.SetStoredProcedure(conn, L"dbo.singup_sp");
	cmd.SetReturnParam();
	cmd.SetInputParam(L"@id", clientID);
	cmd.SetInputParam(L"@password", clientPS);
	cmd.SetInputParam(L"@local", local);
	cmd.SetInputParam(L"@domain", domain);
	cmd.SetOutputParam(L"@global_id", DataTypeEnum::adInteger, sizeof(int32));

	ADORecordset rs;
	cmd.ExecuteStoredProcedure(rs, EExcuteReturnType::Wait_Return);

	
	Protocol::S2C_Singup singupPacket;
	int32 ret = cmd.GetReturnParam();
	if (ret != 0)
	{
		singupPacket.set_error(ret);
	}
	else
	{
		singupPacket.set_error(ret);
		int32 global_id = cmd.GetOutputParam(L"@global_id");
		playerState->SetGlobalID(global_id);
	}

	SendBufferPtr sendBuffer = IdentityServerPacketHandler::MakeSendBuffer(session, singupPacket);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C2S_EmailVerified(PacketSessionPtr& session, Protocol::C2S_EmailVerified& pkt)
{
	bool valid = session->IsValid();
	if (false == valid)
	{
		return false;
	}

	PlayerStatePtr playerState = std::static_pointer_cast<IdentityPlayerState>(session);
	if (nullptr == playerState)
	{
		return false;
	}

	ADOVariant global_id = playerState->GetGlobalID();
	if (static_cast<int32>(global_id) == -1)
	{
		return false;
	}
	ADOVariant verify = atoi(pkt.verified().c_str());

	ADOConnectionInfo ConnectionInfo(L"SQLOLEDB", L"APEIROGON", L"account_database", L"SSPI", L"NO", L"apeirogon", L"1248", EDBMSTypes::MSSQL);
	ADOConnection conn;
	conn.Open(ConnectionInfo);

	ADOCommand cmd;
	cmd.SetStoredProcedure(conn, L"dbo.check_email_verify_sp");
	cmd.SetReturnParam();
	cmd.SetInputParam(L"@global_id", global_id);
	cmd.SetInputParam(L"@verify", verify);

	ADORecordset rs;
	cmd.ExecuteStoredProcedure(rs, EExcuteReturnType::Wait_Return);

	Protocol::S2C_EmailVerified emailVerifiedPacket;
	int32 ret = cmd.GetReturnParam();
	if (ret != 0)
	{
		emailVerifiedPacket.set_error(ret);
	}
	else
	{
		emailVerifiedPacket.set_error(ret);
	}

	SendBufferPtr sendBuffer = IdentityServerPacketHandler::MakeSendBuffer(session, emailVerifiedPacket);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C2S_LoadServer(PacketSessionPtr& session, Protocol::C2S_LoadServer& pkt)
{

	bool valid = session->IsValid();
	if (false == valid)
	{
		return false;
	}

	PlayerStatePtr playerState = std::static_pointer_cast<IdentityPlayerState>(session);
	if (nullptr == playerState)
	{
		return false;
	}

	return true;
}

bool Handle_C2S_LoadCharacters(PacketSessionPtr& session, Protocol::C2S_LoadCharacters& pkt)
{
	bool valid = session->IsValid();
	if (false == valid)
	{
		return false;
	}

	PlayerStatePtr playerState = std::static_pointer_cast<IdentityPlayerState>(session);
	if (nullptr == playerState)
	{
		return false;
	}

	ADOConnectionInfo ConnectionInfo(L"SQLOLEDB", L"APEIROGON", L"game_database", L"SSPI", L"NO", L"apeirogon", L"1248", EDBMSTypes::MSSQL);
	ADOConnection conn;
	conn.Open(ConnectionInfo);

	ADOVariant global_id = playerState->GetGlobalID();
	ADOVariant server_id = 1;

	ADOCommand cmd;
	cmd.SetStoredProcedure(conn, L"dbo.load_character_sp");
	cmd.SetReturnParam();
	cmd.SetInputParam(L"@global_id", global_id);
	cmd.SetInputParam(L"@server_id", server_id);

	ADORecordset rs;
	cmd.ExecuteStoredProcedure(rs, EExcuteReturnType::Wait_Return);


	Protocol::S2C_LoadCharacters loadCharacterPackets;
	if (rs.IsOpen())
	{
		while (!(rs.IsEof()))
		{
			std::string name = rs.GetFieldItem(L"name");
			int32		level = rs.GetFieldItem(L"level");
			int32		position = rs.GetFieldItem(L"position");
			int32		job = rs.GetFieldItem(L"class");
			int32		tribe = rs.GetFieldItem(L"tribe");

			uint32		skin =		static_cast<uint32>(rs.GetFieldItem(L"skin"));
			uint32		hair =		static_cast<uint32>(rs.GetFieldItem(L"hair"));
			uint32		eye =		static_cast<uint32>(rs.GetFieldItem(L"eye"));
			uint32		eyebrow =	static_cast<uint32>(rs.GetFieldItem(L"eyebrow"));

			Protocol::SCharacterData* loadCharacter = loadCharacterPackets.add_character();
			loadCharacter->set_name(name);
			loadCharacter->set_level(level);
			loadCharacter->set_position(position);
			loadCharacter->set_job(job);
			loadCharacter->set_tribe(tribe);

			loadCharacter->set_skin(skin);
			loadCharacter->set_hair(hair);
			loadCharacter->set_eye(eye);
			loadCharacter->set_eyebrow(eyebrow);

			rs.MoveNext();
		}
	}

	SendBufferPtr sendBuffer = IdentityServerPacketHandler::MakeSendBuffer(session, loadCharacterPackets);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C2S_SelectServer(PacketSessionPtr& session, Protocol::C2S_SelectServer& pkt)
{
	return false;
}

bool Handle_C2S_CreateCharacter(PacketSessionPtr& session, Protocol::C2S_CreateCharacter& pkt)
{
	bool valid = session->IsValid();
	if (false == valid)
	{
		return false;
	}

	PlayerStatePtr playerState = std::static_pointer_cast<IdentityPlayerState>(session);
	if (nullptr == playerState)
	{
		return false;
	}

	ADOVariant name				= pkt.character().name().c_str();
	ADOVariant job				= pkt.character().job();
	ADOVariant position			= pkt.character().position();
	ADOVariant tribe			= pkt.character().tribe();
	ADOVariant global_id		= playerState->GetGlobalID();
	ADOVariant server_id		= 1;

	ADOVariant skin				= static_cast<int32>(pkt.character().skin());
	ADOVariant hair				= static_cast<int32>(pkt.character().hair());
	ADOVariant eye				= static_cast<int32>(pkt.character().eye());
	ADOVariant eyebrow			= static_cast<int32>(pkt.character().eyebrow());

	ADOConnectionInfo ConnectionInfo(L"SQLOLEDB", L"APEIROGON", L"game_database", L"SSPI", L"NO", L"apeirogon", L"1248", EDBMSTypes::MSSQL);
	ADOConnection conn;
	conn.Open(ConnectionInfo);

	ADOCommand cmd;
	cmd.SetStoredProcedure(conn, L"dbo.create_character_sp");
	cmd.SetReturnParam();
	cmd.SetInputParam(L"@name", name);
	cmd.SetInputParam(L"@class", job);
	cmd.SetInputParam(L"@position", position);
	cmd.SetInputParam(L"@tribe", tribe);
	cmd.SetInputParam(L"@global_id", global_id);
	cmd.SetInputParam(L"@server_id", server_id);

	cmd.SetInputParam(L"@skin", skin);
	cmd.SetInputParam(L"@hair", hair);
	cmd.SetInputParam(L"@eye", eye);
	cmd.SetInputParam(L"@eyebrow", eyebrow);

	ADORecordset rs;
	cmd.ExecuteStoredProcedure(rs, EExcuteReturnType::Wait_Return);

	Protocol::S2C_CreateCharacter createCharacterPacket;
	int32 ret = cmd.GetReturnParam();
	if (ret != 0)
	{
		createCharacterPacket.set_error(ret);
	}
	else
	{
		createCharacterPacket.set_error(ret);
	}

	SendBufferPtr sendBuffer = IdentityServerPacketHandler::MakeSendBuffer(session, createCharacterPacket);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C2S_TravelServer(PacketSessionPtr& session, Protocol::C2S_TravelServer& pkt)
{
	return true;
}