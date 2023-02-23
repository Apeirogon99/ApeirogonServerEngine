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
			std::string name		= rs.GetFieldItem(L"name");
			int32	job				= rs.GetFieldItem(L"class");
			int32	tribe			= rs.GetFieldItem(L"tribe");
			int32	level			= rs.GetFieldItem(L"level");
			int32	position		= rs.GetFieldItem(L"position");

			uint32	body_color		= static_cast<uint32>(rs.GetFieldItem(L"body_color"));
			uint32	hair_color		= static_cast<uint32>(rs.GetFieldItem(L"hair_color"));
			uint32	eye_color		= static_cast<uint32>(rs.GetFieldItem(L"eye_color"));

			int32	head			= rs.GetFieldItem(L"head");
			int32	ears			= rs.GetFieldItem(L"ears");
			int32	feet			= rs.GetFieldItem(L"feet");
			int32	hair			= rs.GetFieldItem(L"hair");
			int32	facials_01		= rs.GetFieldItem(L"facials_01");
			int32	facials_02		= rs.GetFieldItem(L"facials_02");
			int32	helmet			= rs.GetFieldItem(L"helmet");
			int32	shoulders		= rs.GetFieldItem(L"shoulders");
			int32	skirt			= rs.GetFieldItem(L"skirt");
			int32	legs			= rs.GetFieldItem(L"legs");
			int32	legs_add		= rs.GetFieldItem(L"legs_add");
			int32	hands			= rs.GetFieldItem(L"hands");
			int32	hands_add		= rs.GetFieldItem(L"hands_add");
			int32	chest			= rs.GetFieldItem(L"chest");
			int32	chest_add		= rs.GetFieldItem(L"chest_add");
			int32	cape			= rs.GetFieldItem(L"cape");
			int32	bracers			= rs.GetFieldItem(L"bracers");
			int32	bracers_add		= rs.GetFieldItem(L"bracers_add");
			int32	boots			= rs.GetFieldItem(L"boots");
			int32	belt			= rs.GetFieldItem(L"belt");
			int32	tabard			= rs.GetFieldItem(L"tabard");
			int32	back_2hl		= rs.GetFieldItem(L"back_2hl");
			int32	back_shield		= rs.GetFieldItem(L"back_shield");
			int32	back_weapon_l	= rs.GetFieldItem(L"back_weapon_l");
			int32	back_weapon_r	= rs.GetFieldItem(L"back_weapon_r");
			int32	back_bow		= rs.GetFieldItem(L"back_bow");
			int32	quiver			= rs.GetFieldItem(L"quiver");
			int32	weapon_r_arrow	= rs.GetFieldItem(L"weapon_r_arrow");
			int32	weapon_shield	= rs.GetFieldItem(L"weapon_shield");
			int32	weapon_l		= rs.GetFieldItem(L"weapon_l");
			int32	weapon_r		= rs.GetFieldItem(L"weapon_r");
			int32	hip_l			= rs.GetFieldItem(L"hip_l");
			int32	hip_r			= rs.GetFieldItem(L"hip_r");


			Protocol::SCharacterData* loadCharacterData = loadCharacterPackets.add_character();
			loadCharacterData->set_name(name);
			loadCharacterData->set_level(level);
			loadCharacterData->set_position(position);
			loadCharacterData->set_job(job);
			loadCharacterData->set_tribe(tribe);

			Protocol::SCharacterAppearance* loadCharacterAppearance = loadCharacterPackets.add_appearance();
			loadCharacterAppearance->set_body_color(body_color);
			loadCharacterAppearance->set_hair_color(hair_color);
			loadCharacterAppearance->set_eye_color(eye_color);
			loadCharacterAppearance->set_head(head);
			loadCharacterAppearance->set_ears(ears);
			loadCharacterAppearance->set_feet(feet);
			loadCharacterAppearance->set_hair(hair);
			loadCharacterAppearance->set_facials_01(facials_01);
			loadCharacterAppearance->set_facials_02(facials_02);
			loadCharacterAppearance->set_helmet(helmet);
			loadCharacterAppearance->set_shoulders(shoulders);
			loadCharacterAppearance->set_skirt(skirt);
			loadCharacterAppearance->set_legs(legs);
			loadCharacterAppearance->set_legs_add(legs_add);
			loadCharacterAppearance->set_hands(hands);
			loadCharacterAppearance->set_hands_add(hands_add);
			loadCharacterAppearance->set_chest(chest);
			loadCharacterAppearance->set_chest_add(chest_add);
			loadCharacterAppearance->set_cape(cape);
			loadCharacterAppearance->set_bracers(bracers);
			loadCharacterAppearance->set_bracers_add(bracers_add);
			loadCharacterAppearance->set_boots(boots);
			loadCharacterAppearance->set_belt(belt);
			loadCharacterAppearance->set_tabard(tabard);
			loadCharacterAppearance->set_back_2hl(back_2hl);
			loadCharacterAppearance->set_back_shield(back_shield);
			loadCharacterAppearance->set_back_weapon_l(back_weapon_l);
			loadCharacterAppearance->set_back_weapon_r(back_weapon_r);
			loadCharacterAppearance->set_back_bow(back_bow);
			loadCharacterAppearance->set_quiver(quiver);
			loadCharacterAppearance->set_weapon_r_arrow(weapon_r_arrow);
			loadCharacterAppearance->set_weapon_shield(weapon_shield);
			loadCharacterAppearance->set_weapon_l(weapon_l);
			loadCharacterAppearance->set_weapon_r(weapon_r);
			loadCharacterAppearance->set_hip_l(hip_l);
			loadCharacterAppearance->set_hip_r(hip_r);

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

	ADOVariant name					= pkt.data().name().c_str();
	ADOVariant job					= pkt.data().job();
	ADOVariant position				= pkt.data().position();
	ADOVariant tribe				= pkt.data().tribe();
	ADOVariant global_id			= playerState->GetGlobalID();
	ADOVariant server_id			= 1;

	ADOVariant body_color			= static_cast<int32>(pkt.appearance().body_color());
	ADOVariant hair_color			= static_cast<int32>(pkt.appearance().hair_color());
	ADOVariant eye_color			= static_cast<int32>(pkt.appearance().eye_color());
	
	ADOVariant head					= pkt.appearance().head();
	ADOVariant ears					= pkt.appearance().ears();
	ADOVariant feet					= pkt.appearance().feet();
	ADOVariant hair					= pkt.appearance().hair();
	//ADOVariant facials_01			= pkt.appearance().facials_01();
	ADOVariant facials_02			= pkt.appearance().facials_02();
	//ADOVariant helmet				= pkt.appearance().helmet();
	//ADOVariant shoulders			= pkt.appearance().shoulders();
	//ADOVariant skirt				= pkt.appearance().skirt();
	ADOVariant legs					= pkt.appearance().legs();
	//ADOVariant legs_add				= pkt.appearance().legs_add();
	ADOVariant hands				= pkt.appearance().hands();
	//ADOVariant hands_add			= pkt.appearance().hands_add();
	ADOVariant chest				= pkt.appearance().chest();
	//ADOVariant chest_add			= pkt.appearance().chest_add();
	//ADOVariant cape					= pkt.appearance().cape();
	ADOVariant bracers				= pkt.appearance().bracers();
	//ADOVariant bracers_add			= pkt.appearance().bracers_add();
	ADOVariant boots				= pkt.appearance().boots();
	//ADOVariant belt					= pkt.appearance().belt();
	//ADOVariant tabard				= pkt.appearance().tabard();
	//ADOVariant back_2hl				= pkt.appearance().back_2hl();
	//ADOVariant back_shield			= pkt.appearance().back_shield();
	//ADOVariant back_weapon_l		= pkt.appearance().back_weapon_l();
	//ADOVariant back_weapon_r		= pkt.appearance().back_weapon_r();
	//ADOVariant back_bow				= pkt.appearance().back_bow();
	//ADOVariant quiver				= pkt.appearance().quiver();
	//ADOVariant weapon_r_arrow		= pkt.appearance().weapon_r_arrow();
	//ADOVariant weapon_shield		= pkt.appearance().weapon_shield();
	//ADOVariant weapon_l				= pkt.appearance().weapon_l();
	//ADOVariant weapon_r				= pkt.appearance().weapon_r();
	//ADOVariant hip_l				= pkt.appearance().hip_l();
	//ADOVariant hip_r				= pkt.appearance().hip_r();


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

	cmd.SetInputParam(L"@body_color", body_color);
	cmd.SetInputParam(L"@hair_color", hair_color);
	cmd.SetInputParam(L"@eye_color", eye_color);

	cmd.SetInputParam(L"@head", head);
	cmd.SetInputParam(L"@ears", ears);
	cmd.SetInputParam(L"@feet", feet);
	cmd.SetInputParam(L"@hair", hair);
	cmd.SetInputParam(L"@facials_02", facials_02);
	cmd.SetInputParam(L"@legs", legs);
	cmd.SetInputParam(L"@hands", hands);
	cmd.SetInputParam(L"@chest", chest);
	cmd.SetInputParam(L"@bracers", bracers);
	cmd.SetInputParam(L"@boots	", boots);

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

bool Handle_C2S_AppearanceCharacter(PacketSessionPtr& session, Protocol::C2S_AppearanceCharacter& pkt)
{
	return false;
}

bool Handle_C2S_DeleteCharacter(PacketSessionPtr& session, Protocol::C2S_DeleteCharacter& pkt)
{
	return false;
}

bool Handle_C2S_UpdateNickName(PacketSessionPtr& session, Protocol::C2S_UpdateNickName& pkt)
{
	return false;
}

bool Handle_C2S_TravelServer(PacketSessionPtr& session, Protocol::C2S_TravelServer& pkt)
{
	return true;
}