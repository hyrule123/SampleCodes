#include <vector>
#include <iostream>
#include <random>

class test
{
public:
	test()
	{
		//std::cout << "Constructor" << std::endl;
		
		int a = rand();
		a = rand();
		testBool = (bool)(rand() % 2);
		std::cout << testBool << std::endl;
	}
	~test()
	{
		std::cout << "Destructor" << std::endl;
	}

	test(const test& _other)
		:testBool(_other.testBool)
	{
		std::cout << "Copy Constructor" << std::endl;
	}

	test(test&& _move) noexcept
		: testBool(_move.testBool)
	{
		std::cout << "Move Constructor" << std::endl;
	}

	void operator = (const test& _other)
	{
		testBool = _other.testBool;
		std::cout << "Copy Assignment" << std::endl;
	}
	void operator = (test&& _move) noexcept
	{
		testBool = _move.testBool;
		std::cout << "Move Assignment" << std::endl;
	}

	bool testBool;
	std::shared_ptr<int> i;
};

int main()
{
	srand(time(0));

	std::cout << "start" << std::endl;
	{
		std::vector<test> testVec(500);

		std::partition(testVec.begin(), testVec.end(),
			[](const test& _test)->bool
			{
				return _test.testBool;
			}
		);

		testVec.clear();

		int b = 3;
	}
	std::cout << "=====================" << std::endl;
	{
		std::vector<test> testVec(500);
		std::erase_if(testVec,
			[](const test& _test)->bool
			{
				return _test.testBool;
			}
		);

		

		int b = 3;
	}



	int a = 3;

	return 0;
}
