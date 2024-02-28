#pragma once
#include <iostream>
#include <vector>

//https://stackoverflow.com/questions/35736568/is-there-a-way-to-check-if-rvo-was-applied

class NRVOTEST
{
public:
	NRVOTEST();
	NRVOTEST(const NRVOTEST& _other);
	NRVOTEST(NRVOTEST&& _move) noexcept;
	~NRVOTEST();


private:
	int a;
};

NRVOTEST NRVO_TESTFUNC(bool _b);

