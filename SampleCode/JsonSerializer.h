#pragma once
#include "Serializer.h"

#include "json.h"
#include "StringConverter.h"

#define MAKE_JSONPAIR(_var) JsonSerializer::JsonPair(#_var, _var)
#define JSON_KEY(_varName) #_varName



	//사용예시: Serialize
	//int var = 3;
	//jSer["key"] = JsonSerializer::Serialize(var);

	//사용예시: DeSerialize
	//int var = 3;
	//JsonSerializer::DeSerialize(jSer["key"], var);

	//불편한 느낌이 없지 않아 있는데, 이 방식으로 해야 복잡하지 않게 사용이 가능함...
	class JsonSerializer : public Serializer
	{
	public:
		JsonSerializer();
		virtual ~JsonSerializer();

		virtual bool SaveFile(std::filesystem::path const& _fullPath) override;
		virtual bool LoadFile(std::filesystem::path const& _fullPath) override;
		
		Json::Value& operator[] (const std::string_view _strKey) { return m_jVal[_strKey]; }

		template <NotPointerTypes T>
		static Json::Value SerializeBase64(const T& _val)
		{
			return Json::Value(StringConverter::Base64Encode(_val));
		}
		template <NotPointerTypes T>
		static void DeSerializeBase64(const Json::Value& _jVal, T& _retType)
		{
			_retType = StringConverter::Base64Decode<T>(_jVal.asString());
		}


		template <typename T> requires std::is_arithmetic_v<T>
		static Json::Value Serialize(T _i) { return Json::Value(_i); }

		template <typename T> requires std::is_arithmetic_v<T>
		static void DeSerialize(const Json::Value& _jVal, T& _retType) { _retType = _jVal.as<T>(); }

		static Json::Value Serialize(const std::string_view _val) { return Json::Value(_val); };
		static void DeSerialize(const Json::Value& _jVal, std::string& _retType) { _retType = _jVal.asString(); };

		static Json::Value Serialize(const std::wstring_view _val)
		{
			Json::Value ret{};
			if (false == _val.empty())
			{
				ret = base64_encode(reinterpret_cast<const unsigned char*>(_val.data()),
					static_cast<unsigned int>(_val.size()) * 2u);
			}
			return ret;
		}
		static void DeSerialize(const Json::Value& _jVal, std::wstring& _val)
		{
			std::string decoded = base64_decode(_jVal.asString());
			_val = std::wstring(reinterpret_cast<const wchar_t*>(decoded.c_str()));
		}


		

	private:
		inline bool CheckValid(const std::string_view _strKey, bool _bCheckValueExist);
		

	private:
		Json::Value m_jVal;
	};

	//지원되는 타입에 한해서 작동하는 템플릿 함수.
	//미지원 타입 또는 커스텀 데이터의 경우에는 SerializeBase64를 사용할것.
	template <typename T>
	inline Json::Value& operator <<(Json::Value& _jVal, const T& _data)
	{
		Json::Value& ret = _jVal;
		if (_jVal.isArray())
		{
			_jVal.append(JsonSerializer::Serialize(_data));
		}
		else
		{
			_jVal = JsonSerializer::Serialize(_data);
		}
		return ret;
	}

	//Json::Value에 대한 템플릿 특수화
	template <>
	inline Json::Value& operator <<(Json::Value& _jVal, const Json::Value& _newjVal)
	{
		Json::Value& ret = _jVal;
		if (_jVal.isArray())
		{
			_jVal.append(_newjVal);
		}
		else
		{
			_jVal = _newjVal;
		}
		return ret;
	}

	//Json::Value가 들어오는 경우에는 굳이 Serialize 단계를 거칠 필요가 없으므로 바로 넣어준다.
	inline Json::Value& operator <<(Json::Value& _jVal, Json::Value&& _newjVal)
	{
		Json::Value& ret = _jVal;
		if (_jVal.isArray())
		{
			_jVal.append(std::move(_newjVal));
		}
		else
		{
			_jVal = std::move(_newjVal);
		}
		return ret;
	}

	template <typename T>
	inline const Json::Value& operator >>(const Json::Value& _jVal, T& _data)
	{
		const Json::Value& ret = _jVal;
		JsonSerializer::DeSerialize(_jVal, _data);
		return ret;
	}
