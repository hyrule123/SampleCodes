#include "JsonSerializer.h"


	JsonSerializer::JsonSerializer()
		: m_jVal(Json::objectValue)
	{
	}

	JsonSerializer::~JsonSerializer()
	{
	}
	bool JsonSerializer::SaveFile(std::filesystem::path const& _fullPath)
	{
		return false;
	}

	bool JsonSerializer::LoadFile(std::filesystem::path const& _fullPath)
	{
		return false;
	}

	bool JsonSerializer::CheckValid(const std::string_view _strKey, bool _bCheckValueExist)
	{
		if (_strKey.empty())
		{
			
			return false;
		}
		else if (_bCheckValueExist && false == m_jVal.isMember(_strKey))
		{
			
			return false;
		}
		return true;
	}
