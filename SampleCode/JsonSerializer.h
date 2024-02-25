#pragma once
#include "Serializer.h"

#include "StringConverter.h"
#include "json.h"
#include <unordered_set>

#include "SimpleMath.h"
#include <utility>


#define MAKE_JSONPAIR(_var) JsonSerializer::JsonPair(#_var, _var)
#define JSON_KEY(_varName) #_varName

extern std::unordered_set<std::string> g_cstrArchive;

using float3 = ehw::math::Vector3;
using float4 = ehw::math::Vector4;
using MATRIX = ehw::math::Matrix;

	class JsonSerializer : public Serializer
	{

	public:
		JsonSerializer();
		virtual ~JsonSerializer();

		virtual eResult SaveFile(std::filesystem::path const& _fullPath) override;
		virtual eResult LoadFile(std::filesystem::path const& _fullPath) override;

		Json::Value& operator[] (const std::string_view _strKey) { return m_jVal[_strKey]; }
		inline const Json::Value& operator[] (const std::string_view _strKey) const { return m_jVal[_strKey]; };
		inline bool isMember(const std::string_view _strKey) const { return m_jVal.isMember(_strKey); }

	private:
		Json::Value m_jVal;
	};

	
	//특수화하고자 하는 타입
	template <typename T>
	concept JsonTypeSpecialization =
		type_traits_Ex::PointerTypes<T> ||
		type_traits_Ex::is_enum_class_v<T> ||
		std::is_same_v<float3, T> ||
		std::is_same_v<float4, T>;

	//생성자로 생성 가능한 타입
	template <typename T>
	concept JsonDefaultTypes =
		!JsonTypeSpecialization<T> &&
		requires (T t) { { Json::Value(t) }; };

	//별도의 특수화가 되어있지 않으면서 Json::Value 생성자가 없을 경우
	template <typename T>
	concept JsonBase64Types =
		!JsonTypeSpecialization<T> &&
		!requires (T t) { { Json::Value(t) }; };

	//template <JsonDefaultTypes T>
	//inline void operator <<(Json::Value& _jVal, const T& _data)
	//{
	//	_jVal = _data;
	//}

	template <JsonDefaultTypes T>
	inline void operator <<(Json::Value& _jVal, T&& _data) noexcept
	{
		_jVal = std::forward<T>(_data);
	}

	//타입을 가리지 않고 포인터 제외 데이터를 Base64로 전환하므로 주의할것.
	//컨테이너는 StringConverter에서 함수를 별도로 만들어줄것.
	//또한 혹시 별도의 처리를 해야 하는 데이터 타입이 있을 경우 이 함수에 대해 템플릿 특수화를 해줄것.
	template <JsonBase64Types T>
	inline void operator <<(Json::Value& _jVal, const T& _data)
	{
		_jVal = StringConverter::Base64Encode(_data);
	}

	//enum class
	template <JsonTypeSpecialization T> requires type_traits_Ex::is_enum_class_v<T>
	inline void operator <<(Json::Value& _jVal, T _data)
	{
		_jVal = static_cast<std::underlying_type_t<T>>(_data);
	}

	template <JsonDefaultTypes T>
	inline void operator >>(const Json::Value& _jVal, T& _data)
	{
		_data = _jVal.as<T>();
	}

	template <JsonBase64Types T>
	inline void operator >>(const Json::Value& _jVal, T& _data)
	{
		_data = StringConverter::Base64Decode<T>(_jVal.asString());
	}


	template <JsonTypeSpecialization T> requires type_traits_Ex::is_enum_class_v<T>
	inline void operator >>(const Json::Value& _jVal, T& _data)
	{
		_data = static_cast<T>(_jVal.as<std::underlying_type_t<T>>());
	}


#pragma region 특수화
	//c_str
	void operator << (Json::Value& _jVal, const char* _c_str);
	void operator >> (const Json::Value& _jVal, const char* _c_str);

	//float3
	void operator << (Json::Value& _jVal, const float3& _f3);
	void operator >> (const Json::Value& _jVal, float3& _f3);

	//float4
	void operator << (Json::Value& _jVal, const MATRIX& _m);
	void operator >> (const Json::Value& _jVal, MATRIX& _m);


#pragma endregion //특수화
