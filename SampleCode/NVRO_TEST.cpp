#include "NVRO_TEST.h"

NRVOTEST::NRVOTEST()
	: a(0)
{
	std::cout << "Created!!" << std::endl;
	//std::cout << "ADDRESS: " << this << "\n" << std::endl;
}

NRVOTEST::NRVOTEST(const NRVOTEST& _other)
	: a(1)
{
	std::cout << "COPIED!!" << std::endl;
	//std::cout << "ADDRESS: " << this << "\n" << std::endl;
}

NRVOTEST::NRVOTEST(NRVOTEST&& _other) noexcept
	: a(2)
{
	std::cout << "MOVED!!" << std::endl;
	//std::cout << "ADDRESS: " << this << "\n" << std::endl;
}

NRVOTEST::~NRVOTEST()
{
	std::cout << "REMOVED!!" << std::endl;
	//std::cout << "ADDRESS: " << this << "\n" << std::endl;
}

NRVOTEST NRVO_TESTFUNC(bool _b)
{
	NRVOTEST ret1{};
	NRVOTEST ret2{};

	std::cout << "Local Value 1 address: " << &ret1 << std::endl;
	std::cout << "Local Value 2 address: " << &ret2 << std::endl;


	if (_b)
	{
		//여길 ret2로 바꾸면 NRVO가 적용되지 않음.
		return ret1;
	}
	else
	{
		return ret1;
	}


	return ret1;
}


