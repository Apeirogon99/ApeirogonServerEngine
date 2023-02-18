#pragma once

enum class EErrorType
{
	NONE = 0,
	INDEX_NONE = 1,
	SMTP = 25,
};

static std::string GetNetworkError(int error)
{
	EErrorType type = static_cast<EErrorType>(error);
	switch (type)
	{
	case EErrorType::NONE:
		return std::string("NONE");
		break;
	case EErrorType::INDEX_NONE:
		return std::string("");
		break;
	case EErrorType::SMTP:
		break;
	default:
		break;
	}
}