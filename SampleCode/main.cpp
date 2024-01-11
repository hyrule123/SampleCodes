#include "Serializer.h"

#include <vector>
#include <string>

class TestClass : public Serializable
{
public:
	TestClass() {}
	~TestClass() {}

	bool Read() override { return true; }
	bool Write() override { return true; }

	std::string GetStrKey() { return "HI"; }
};

class Independent
{
	Independent() {};
	~Independent() {};
};

int main()
{
	int a = 3;
	int b = 10324;

	std::vector<float> vecflt = { 1.f, 2.f, 3.f };

	BinarySerializer ser;
	ser << a << b;
	ser << vecflt;

	std::string str = "test";
	ser << str;
	std::wstring wstr = L"testwstr";
	ser << wstr;

	std::vector<std::string> vecstr = { "1", "2", "3" };
	ser << vecstr;
	
	ser >> b >> a;
	vecflt.clear();
	str.clear();
	wstr.clear();
	ser >> vecflt;
	ser >> str;
	ser >> wstr;

	vecstr.clear();
	ser >> vecstr;

	
	return 0;
}
