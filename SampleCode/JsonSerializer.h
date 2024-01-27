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

		virtual eResult SaveFile(std::filesystem::path const& _fullPath) override;
		virtual eResult LoadFile(std::filesystem::path const& _fullPath) override;
		
		Json::Value& operator[] (const std::string_view _strKey) { return m_jVal[_strKey]; }

		//타입을 가리지 않고 일단 데이터를 집어넣으므로 주의
		template <type_traits_Ex::NotPointerTypes T>
		static void SerializeBase64(Json::Value& _jVal, const T& _val)
		{
			_jVal << StringConverter::Base64Encode(_val);
		}
		//타입을 가리지 않고 일단 데이터를 집어넣으므로 주의
		template <type_traits_Ex::NotPointerTypes T>
		static void DeSerializeBase64(const Json::Value& _jVal, T& _val)
		{
			_val = StringConverter::Base64Decode<T>(_jVal.asString());
		}

	private:
		Json::Value m_jVal;
	};

	template <typename T>
	concept JsonDefaultTypes = 
		!type_traits_Ex::PointerTypes<T> &&
		requires (T t)
	{
		{ Json::Value(t) };
	};
	template <typename T>
	concept NotJsonDefaultTypes = 
		!type_traits_Ex::PointerTypes<T> &&
		!requires (T t)
	{
		{ Json::Value(t) };
	};
	template <JsonDefaultTypes T>
	inline void operator <<(Json::Value& _jVal, const T& _data)
	{
		_jVal = _data;
	}
	template <JsonDefaultTypes T>
	inline void operator <<(Json::Value& _jVal, T&& _data)
	{
		_jVal = std::move(_data);
	}

	inline void operator <<(Json::Value& _jVal, const char* _data)
	{
		if (nullptr != _data)
		{
			_jVal = Json::StaticString(_data);
		}
	}


	//타입을 가리지 않고 포인터 제외 데이터를 Base64로 전환하므로 주의할것.
	//컨테이너는 StringConverter에서 함수를 별도로 만들어줄것.
	//또한 혹시 별도의 처리를 해야 하는 데이터 타입이 있을 경우 이 함수에 대해 템플릿 특수화를 해줄것.
	template <NotJsonDefaultTypes T>
	inline void operator <<(Json::Value& _jVal, const T& _data)
	{
		_jVal = StringConverter::Base64Encode(_data);
	}




	template <JsonDefaultTypes T>
	inline void operator >>(const Json::Value& _jVal, T& _data)
	{
		_data = _jVal.as<T>();
	}

	template <NotJsonDefaultTypes T>
	inline void operator >>(const Json::Value& _jVal, T& _data)
	{
		_data = StringConverter::Base64Decode<T>(_jVal.asString());
	}
