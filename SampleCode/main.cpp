#include "JsonSerializer.h"
#include "BinarySerializer.h"

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

template <typename P, typename T>
concept points_to = 
requires(P p)
{
	{ *p } -> std::common_reference_with<T&>;
} &&
std::equality_comparable_with<std::nullptr_t, P>;

template <typename P>
concept PointerType = requires(P p) { { *p }; } &&
std::equality_comparable_with<std::nullptr_t, P>;

struct ScalarData
{
	int a;
	float b;
	double c;
	bool d;
};

class TestClass
	: public Serializable<BinarySerializer>
	, public Serializable<JsonSerializer>
{
public:
	TestClass();
	virtual ~TestClass();

	virtual std::unique_ptr<TestClass> Clone() { return std::make_unique<TestClass>((*this)); }

	virtual eResult Serialize(JsonSerializer& _ser) override;
	virtual eResult DeSerialize(JsonSerializer& _ser) override;
	virtual eResult Serialize(BinarySerializer& _ser) override;
	virtual eResult DeSerialize(BinarySerializer& _ser) override;

};


void JsonPrecisionCheck();

enum e
{
	a,
	b
};

enum class ec
{
	a, b
};

bool isMember(const std::vector<std::string_view>& _vec)
{
	return !_vec.empty();
}

bool testfunc(float* f) { return true; }
bool testfunc(int* f) { return false; }

class intcont
{
public:
	intcont() : i() {}

	template <typename T>
	void set(const T& t) { i = (int)t; }


	template <typename T> requires std::is_rvalue_reference_v<T>
	void set(T&& t) noexcept { i = (int)std::move(t); }


private:
	int i;
};

class teststream
{
public:
	teststream() {};
	~teststream() {};


};

std::ostream& operator << (std::ostream& os, teststream t)
{
	int i = 3;

	return os;
}
std::istream& operator >> (std::istream& is, teststream t)
{
	int j = 3;

	return is;
}

template <typename Derived, typename Base>
concept Decay_Derived_From = std::derived_from<std::decay_t<Derived>, std::decay_t<Base>>;

template <typename T>
concept is_serializable = requires (std::ostream _ofs, std::istream _ifs, T _t)
{
	{ _ofs << _t } -> Decay_Derived_From<std::ostream>;
	{ _ifs >> _t } -> Decay_Derived_From<std::istream>;
};

template <typename T>
class forwardDeclClass
{
public:
	forwardDeclClass() : forwardPtr() {};
	~forwardDeclClass() {};

	T* forwardPtr;
};
class nonexistClass;
int main()
{
	forwardDeclClass<nonexistClass> cls{};
	

	constexpr bool decayderfrom = Decay_Derived_From<const std::ostream&, std::ostream>;

	constexpr bool serable = is_serializable<Json::Value>;
	constexpr bool serable2 = is_serializable<teststream>;

	constexpr bool serable3 = is_serializable<BinarySerializer>;

	
	using deleteall = std::remove_cv_t<std::remove_pointer_t<std::decay_t<const int*&&>>>;
	using commontype = std::common_type_t<const int*&& const>;

	std::ifstream ifstest;
	teststream testream{};
	ifstest >> testream;

	ifstest.open("./here.json", std::ios::binary);
	Json::Value jsonvaluetest;
	ifstest >> jsonvaluetest;

	size_t size = jsonvaluetest.size();

	constexpr bool isderivedFrom = std::derived_from <const std::ofstream&, std::ostream>;
	using typetype = std::decay_t<const std::ofstream&>;

	float fff = 3.f;
	intcont ic{};
	ic.set(fff);


	{
		Json::Value matTest{};
		MATRIX mat
		{
		1.f, 2.f, 3.f, 4.f,
		5.f, 6.f, 7.f, 8.f,
		9.f, 10.f, 11.f, 12.f,
		13.f, 14.f, 15.f, 16.f
		};

		matTest << mat;

		mat = MATRIX::Identity;

		matTest >> mat;

		int a = 3;
	}
	


	float ff = 3.f;
	bool result = testfunc(&ff);
	int ii = 1;
	result = testfunc(&ii);

	try
	{
		Json::Value objVal;
		objVal["OBJ"] = 1;

		objVal.append(1);
	}
	catch (const std::exception& _err)
	{
		std::string errmsg = _err.what();
	}


	{
		JsonSerializer ser{};

		float3 f3 = { 1.f, 2.f, 3.f };
		float4 f4 = { 1.f, 2.f, 3.f, 4.f };
		ser["float3"] << f3;
		

		ser.SaveFile("./float34Test.json");
	}



	Json::Value jval2 = Json::nullValue;
	std::string teststr = jval2.asString();

	using naked = std::remove_cvref_t<const std::string&>;
	using naked2 = std::remove_cvref_t<std::string&>;
	using naked3 = std::decay_t<const std::string&>;
	using naked4 = std::remove_cvref_t<const std::string*&>;




	//concept 작동 확인
	constexpr bool testbool1 = JsonDefaultTypes<int>;
	constexpr bool testbool2 = JsonDefaultTypes<int*>;
	constexpr bool testbool3 = JsonDefaultTypes<ScalarData>;
	constexpr bool testbool4 = JsonDefaultTypes<size_t>;
	constexpr bool testbool5 = JsonDefaultTypes<double>;

	

	std::string str{};

	str += (char)0b11110100;
	str += (char)0b11011111;
	str += (char)0b01111101;
	
	ScalarData mydata{};
	mydata.a = 1;
	mydata.b = 2.f;
	mydata.c = 3.0;
	mydata.d = true;
	std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(&mydata), sizeof(mydata));

	mydata = ScalarData{};
	base64_decode(encoded, reinterpret_cast<unsigned char*>(&mydata), sizeof(mydata));
	
	ScalarData sd{};
	//JsonPrecisionCheck();

	{
		constexpr bool bt = std::is_enum_v<ScalarData>;
		JsonSerializer ser;
		ser["INT"] << 3;

		ser["CUSTOM"] << sd;

		int i = 0;
		ser["INT"] >> i;

		ser["FLOAT"] << 3.f;
		float f{};
		ser["FLOAT"] >> f;

		ser["DOUBLE"] << 4.0;
		double d{};
		ser["DOUBLE"] >> d;

		//이건 에러가 발생해야함
		int* pInt{};
		//ser["POINTERNOTSUPPORT"] << pInt;

		//이것도 에러가 발생해야함
		std::shared_ptr<int> pSharedInt{};
		//ser["SHAREDPOINTERSUPPORT"] << pSharedInt;

		const char* pstr = "string";
		ser["static string"] << pstr;

		ser["INT_RVALUE"] << 3;
		i = 0;
		ser["INT_RVALUE"] >> i;

		ScalarData scalarData;
		scalarData.a = 4;
		scalarData.b = 3.f;
		scalarData.c = 2.0;
		scalarData.d = true;

		//커스텀된 데이터는 Template 함수 SerializeBase64 함수를 통해서 넣을 수 있음
		//JsonSerializer::SerializeBase64(ser["BASE64"], scalarData);
		scalarData = ScalarData{};
		//JsonSerializer::DeSerializeBase64(ser["BASE64"], scalarData);
		
		//또는 직접 처리도 가능
		ser["BASE64_2"] << StringConverter::Base64Encode(scalarData);
		scalarData = ScalarData{};
		std::string base64Data = ser["BASE64_2"].asString();
		scalarData = StringConverter::Base64Decode<ScalarData>(base64Data);
		
		//std::vector 등의 컨테이너: Json::arrayValue로 초기화된 Json::Value 변수에 저장
		std::vector<ScalarData> vecScalar;
		for (int i = 0; i < 10; ++i)
		{
			scalarData.a = 4 + i;
			scalarData.b = (float)(3 + i);
			scalarData.c = (double)(2 + i);
			scalarData.d = (bool)(i % 2);
			vecScalar.push_back(scalarData);
		}

		{
			ser["Vector"] = Json::arrayValue;
			Json::Value& vectorVal = ser["Vector"];

			//참고 - json 내부에서 수정될 일이 없으면 아예 Base64를 통해 통으로 저장해도 됨.
			vectorVal.resize(vectorVal.size());
			for (size_t i = 0; i < vecScalar.size(); ++i)
			{
				vectorVal[i][0] << vecScalar[i].a;
				vectorVal[i][1] << vecScalar[i].b;
				vectorVal[i][2] << vecScalar[i].c;
				vectorVal[i][3] << vecScalar[i].d;

				//base64형태로 저장 예시
				vectorVal[i][4] << vecScalar[i];
			}
		}


		vecScalar.clear();
		
		{
			const Json::Value& vectorVal = ser["Vector"];
			Json::Value::ArrayIndex size = vectorVal.size();
			vecScalar.resize(size);
			for (Json::Value::ArrayIndex i = 0; i < size; ++i)
			{
				vectorVal[i][0] >> vecScalar[i].a;
				vectorVal[i][1] >> vecScalar[i].b;
				vectorVal[i][2] >> vecScalar[i].c;
				vectorVal[i][3] >> vecScalar[i].d;

				//base64형태로 저장 예시
				vectorVal[i][4] >> vecScalar[i];
			}

			__debugbreak();
		}
	}

	TestClass testc{};
	testc.Serializable<BinarySerializer>::SaveFile("test");
	testc.Serializable<JsonSerializer>::SaveFile("test");
	testc.Serializable<BinarySerializer>::LoadFile("test");
	testc.Serializable<JsonSerializer>::LoadFile("test");
	
	return 0;
}

void JsonPrecisionCheck()
{

	//초기값 설정 후 저장
	Json::Value jFloatSave;

	jFloatSave["FLOAT"] = 1234567890987654321.f;
	jFloatSave["DOUBLE"] = 1234567890987654321.0;

	float origF = jFloatSave["FLOAT"].asFloat();
	double origD = jFloatSave["DOUBLE"].asDouble();

	{
		std::ofstream ofs("./floatTest.json");
		if (ofs.is_open())
		{
			ofs << jFloatSave;
			ofs.close();
		}
	}

	//루프 횟수 지정
	int loopend = 1000;
	for (int i = 0; i < loopend; ++i)
	{
		{
			Json::Value jFloat;

			//파일 오픈
			std::ifstream ifs("./floatTest.json");
			if (ifs.is_open())
			{
				ifs >> jFloat;
				ifs.close();
			}

			float f{};
			double d{};

			//JSON으로부터 float값을 받고
			f = jFloat["FLOAT"].asFloat();
			d = jFloat["DOUBLE"].asDouble();

			//그걸 도로 JSON에 집어넣는다.
			jFloat["FLOAT"] = f;
			jFloat["DOUBLE"] = d;

			//다시 저장
			std::ofstream ofs("./floatTest.json");
			if (ofs.is_open())
			{
				ofs << jFloat;
				ofs.close();
			}

			//마지막 루프 떄 값을 확인
			if (loopend - 1 == i)
			{
				float diffF = origF - f;
				double diffD = origD - d;

				//두 부동소수점이 모두 EPSILON 이내 값인지 확인
				if (FLT_EPSILON > diffF)
				{
					int check = 1;
				}

				if (DBL_EPSILON > diffD)
				{
					int check = 1;
				}

				int a = 3;
			}
		}
	}
}

TestClass::TestClass()
{
}

TestClass::~TestClass()
{
}

eResult TestClass::Serialize(JsonSerializer& _ser)
{
	return eResult::Success;
}

eResult TestClass::DeSerialize(JsonSerializer& _ser)
{
	return eResult::Success;
}

eResult TestClass::Serialize(BinarySerializer& _ser)
{
	return eResult::Success;
}

eResult TestClass::DeSerialize(BinarySerializer& _ser)
{
	return eResult::Success;
}
