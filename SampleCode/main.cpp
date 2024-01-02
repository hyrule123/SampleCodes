#include <string>

class test
	: public std::string
{
public:
	//inherit constructor from std::string
	using std::string::string;

	~test() {};

	std::string DoSomeJob();
};


int main()
{
	test tst = "hello";

	tst.DoSomeJob();

	return 0;
}

std::string test::DoSomeJob()
{
	return std::string(this->c_str());
}
