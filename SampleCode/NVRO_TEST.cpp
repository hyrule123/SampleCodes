#include "NVRO_TEST.h"

NRVOTEST::NRVOTEST()
	: a(0)
{
	std::cout << "Created!!" << std::endl;
}

NRVOTEST::NRVOTEST(const NRVOTEST& _other)
	: a(1)
{
	std::cout << "COPIED!!" << std::endl;
}

NRVOTEST::NRVOTEST(NRVOTEST&& _other) noexcept
	: a(2)
{
	std::cout << "MOVED!!" << std::endl;
}

NRVOTEST::~NRVOTEST()
{
	std::cout << "REMOVED!!" << std::endl;
}

NRVOTEST NRVO_TESTFUNC(bool _b)
{
	NRVOTEST tst{};
	std::cout << "ADDRESS: " << reinterpret_cast<size_t>(&tst) << std::endl;

	if (_b)
	{
		return tst;
	}


	return tst;
}

std::vector<NRVOTEST> VECTOR_NRVO_TESTFUNC(NRVOTEST&& test, bool bInsert) noexcept
{
	std::vector<NRVOTEST> ret;

	if (bInsert)
	{
		ret.push_back(std::move(test));
		ret.resize(5);
		return ret;
	}
	else
	{
		ret.resize(7);
		return ret;
	}



	return ret;
}


