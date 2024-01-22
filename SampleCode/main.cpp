#include "Serializer.h"
#include "json.h"
#include "base64.h"

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>



class StringWrapper
{
public:
	StringWrapper() {}
	StringWrapper(const std::string& _str) : m_str(_str) {}
	StringWrapper(const std::string_view _str) : m_str(_str) {}
	
	~StringWrapper() {}
	operator std::string&() { return m_str; }
	operator const std::string& () const { return m_str; }

private:
	std::string m_str;
};


int main()
{
	std::string str = "HI";
	StringWrapper wrap = str;
	str += wrap;
	
	Json::Value j = 3.f;

	float flt = 0.f;
	flt << j;
	
	return 0;
}
