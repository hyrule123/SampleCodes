#include "NVRO_TEST.h"


int main()
{
	NRVOTEST nvro = NRVO_TESTFUNC(true);
	std::cout << "ADDRESS: " << reinterpret_cast<size_t>(&nvro) << std::endl;

	std::vector<NRVOTEST> vecNVRO = VECTOR_NRVO_TESTFUNC(NRVOTEST(), false);

	return 0;
}



