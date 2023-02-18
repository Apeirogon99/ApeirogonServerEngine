#pragma once

//0000 ~ 1000 = 공통
//1000 ~ 2000 = 계정
//2000 ~ 3000 = 게임

enum class EDBErrorType
{
	FAILURE = -1,
	SUCCESS = 0,
	INDEX_NONE = 1,
	SMTP = 25,

	NOT_EXISTS_ID = 1001,
	ID_DISTINCT = 1002,
	EMAIL_DISTINCT = 1003,
	EMAIL_NOT_VERIFY = 1004,
	ALREADY_EMAIL_VERIFY = 1005,
	NOT_SAME_VERIFY = 1006,

	CHARACTER_MAX = 2001,
	POSTION_DISTINCT = 2002,
	NAME_DISTINCT = 2003,
	CHARACTER_NONE = 2004,
};

static std::string GetDatabaseError(int32 error)
{
	EDBErrorType type = static_cast<EDBErrorType>(error);

	switch (type)
	{
	case EDBErrorType::FAILURE:
		return std::string("알 수 없는 에러");
		break;
	case EDBErrorType::SUCCESS:
		return std::string("성공");
		break;
	case EDBErrorType::INDEX_NONE:
		return std::string("해당 인덱스가 존재하지 않습니다.");
		break;
	case EDBErrorType::SMTP:
		return std::string("SMTP 에러");
		break;
	case EDBErrorType::NOT_EXISTS_ID:
		return std::string("아이디가 존재하지 않습니다.");
		break;
	case EDBErrorType::ID_DISTINCT:
		return std::string("해당 아이디가 존재합니다.");
		break;
	case EDBErrorType::EMAIL_DISTINCT:
		return std::string("해당 이메일이 존재합니다.");
		break;
	case EDBErrorType::EMAIL_NOT_VERIFY:
		return std::string("이메일 인증이 되어있지 않습니다.");
		break;
	case EDBErrorType::ALREADY_EMAIL_VERIFY:
		return std::string("이미 인증을 하였습니다.");
		break;
	case EDBErrorType::NOT_SAME_VERIFY:
		return std::string("인증 번호가 다릅니다.");
		break;
	case EDBErrorType::CHARACTER_MAX:
		return std::string("생성할 수 있는 캐릭터를 초과하였습니다.");
		break;
	case EDBErrorType::POSTION_DISTINCT:
		return std::string("이미 같은 자리에 캐릭터가 존재합니다.");
		break;
	case EDBErrorType::NAME_DISTINCT:
		return std::string("이미 존재하는 이름입니다.");
		break;
	case EDBErrorType::CHARACTER_NONE:
		return std::string("캐릭터가 존재하지 않습니다.");
		break;
	default:
		return std::string("알 수 없는 에러");
		break;
	}
}

static int32 GetDatabaseErrorToInt(EDBErrorType inError)
{
	return static_cast<int32>(inError);
}