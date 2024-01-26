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

		//타입을 가리지 않고 일단 데이터를 집어넣으므로 주의
		template <NotPointerTypes T>
		static void SerializeBase64(Json::Value& _jVal, const T& _val)
		{
			_jVal << StringConverter::Base64Encode(_val);
		}
		//타입을 가리지 않고 일단 데이터를 집어넣으므로 주의
		template <NotPointerTypes T>
		static void DeSerializeBase64(const Json::Value& _jVal, T& _val)
		{
			_val = StringConverter::Base64Decode<T>(_jVal.asString());
		}		

	private:
		Json::Value m_jVal;
	};

	//template <typename T> requires std::is_arithmetic_v<T>
	//inline void operator <<(Json::Value& _jVal, T _data)
	//{
	//	_jVal = Json::Value(_data);
	//}


	//지원 안되는 타입은 여기서 다 컷 당함
	//지원하고자 하는 타입이 있을 경우 템플릿 특수화 기능을 통해서 추가하면 됨
	template <NotPointerTypes T>
	inline void operator <<(Json::Value& _jVal, const T& _data)
	{
		//Json::Value에서 지원하는 타입이 있을 시 오버로딩된 생성자로 자동 생성해서 집어넣는다.
		_jVal = Json::Value(_data);
	}
	

	template <NotPointerTypes T>
	inline void operator <<(Json::Value& _jVal, T&& _data)
	{
		//Json::Value에서 지원하는 타입이 있을 시 오버로딩된 생성자로 자동 생성해서 집어넣는다.
		_jVal = Json::Value(std::move(_data));
	}

	template <NotPointerTypes T>
	inline void operator >>(const Json::Value& _jVal, T& _data)
	{
		_data = _jVal.as<T>();
	}
