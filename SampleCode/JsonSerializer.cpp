#include "JsonSerializer.h"


	JsonSerializer::JsonSerializer()
		: m_jVal(Json::objectValue)
	{
	}

	JsonSerializer::~JsonSerializer()
	{
	}
	eResult JsonSerializer::SaveFile(std::filesystem::path const& _fullPath)
	{
		return eResult();
	}

	eResult JsonSerializer::LoadFile(std::filesystem::path const& _fullPath)
	{
		return eResult();
	}
