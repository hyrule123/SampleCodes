#include "JsonSerializer.h"

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

void JsonPrecisionCheck();

int main()
{
	
	constexpr bool pt = points_to<int*, int>;
	constexpr bool ispt = PointerType<std::shared_ptr<int>&>;
	constexpr bool ispt2 = NotPointerTypes<int*>;

	//JsonPrecisionCheck();

	{
		JsonSerializer ser;
		ser["INT"] << 3;

		int i = 0;
		ser["INT"] >> i;

		ser["FLOAT"] << 3.f;
		float f{};
		ser["FLOAT"] >> f;

		ser["DOUBLE"] << 4.0;
		double d{};
		ser["DOUBLE"] >> d;

		int* pInt{};
		//이건 에러가 발생해야함
		//ser["POINTERNOTSUPPORT"] << pInt;

		std::shared_ptr<int> pSharedInt{};
		//ser["SHAREDPOINTERSUPPORT"] << pSharedInt;

		ser["INT_RVALUE"] << 3;

		//struct testStruct
		//{
		//	int a;
		//	float b;
		//} teStruct;
		//ser["TESTSTRUCT"] << teStruct;

		ser["JVAL"] << Json::Value();
	}

	
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
