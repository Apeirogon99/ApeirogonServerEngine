#pragma once

//0000 ~ 1000 = ����
//1000 ~ 2000 = ����
//2000 ~ 3000 = ����

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

static std::string GetDatabaseError(int error)
{
	EDBErrorType type = static_cast<EDBErrorType>(error);

	switch (type)
	{
	case EDBErrorType::FAILURE:
		return std::string("FAILURE");
		break;
	case EDBErrorType::SUCCESS:
		return std::string("SUCCESS");
		break;
	case EDBErrorType::INDEX_NONE:
		return std::string("");
		break;
	case EDBErrorType::NAME_DISTINCT:
		return std::string("ĳ���� �г����� �ߺ��Ǿ����ϴ�");
		break;
	case EDBErrorType::SMTP:
		break;
	default:
		return std::string("�� �� ���� �����Դϴ�");
		break;
	}
}