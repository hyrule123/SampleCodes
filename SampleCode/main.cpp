#include "NVRO_TEST.h"


int main()
{
	NRVOTEST nvro = NRVO_TESTFUNC(true);
	std::cout << "Returned Value address: " << &nvro << std::endl;
	__debugbreak();
	return 0;
}



