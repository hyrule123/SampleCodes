#include "JsonSerializer.h"

#include <fstream>

std::unordered_set<std::string> g_cstrArchive{};

	JsonSerializer::JsonSerializer()
		: m_jVal(Json::objectValue)
	{
	}

	JsonSerializer::~JsonSerializer()
	{
	}
	eResult JsonSerializer::SaveFile(std::filesystem::path const& _fullPath)
	{
		std::ofstream ofs(_fullPath);

		if (false == ofs.is_open())
		{
			return eResult::Fail_Open;
		}

		ofs << m_jVal;

		return eResult::Success;
	}

	void operator<<(Json::Value& _jVal, const char* _c_str)
	{
		if (nullptr != _c_str)
		{
			_jVal = Json::StaticString(_c_str);
		}
	}
	void operator>>(const Json::Value& _jVal, const char* _c_str)
	{
		const auto& pair = g_cstrArchive.insert(_jVal.asString());
		_c_str = pair.first->c_str();
	}

	void operator<<(Json::Value& _jVal, const float3& _f3)
	{
		_jVal.clear();
		_jVal.append(_f3.x);
		_jVal.append(_f3.y);
		_jVal.append(_f3.z);
	}

	void operator>>(const Json::Value& _jVal, float3& _f3)
	{
		if (3 != _jVal.size())
		{
			return;
		}

		_f3.x = _jVal[0].asFloat();
		_f3.y = _jVal[1].asFloat();
		_f3.z = _jVal[2].asFloat();
	}

	void operator<<(Json::Value& _jVal, const MATRIX& _m)
	{
		_jVal.clear();
		_jVal.resize(4);
		for (int i = 0; i < 4; ++i)
		{
			Json::Value& col = _jVal[i];
			col.resize(4);
			for (int j = 0; j < 4; ++j)
			{
				col[j] = _m.m[i][j];
			}
		}
	}
	void operator>>(const Json::Value& _jVal, MATRIX& _m)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				_m.m[i][j] = _jVal[i][j].asFloat();
			}
		}
	}

	eResult JsonSerializer::LoadFile(std::filesystem::path const& _fullPath)
	{
		return eResult();
	}
