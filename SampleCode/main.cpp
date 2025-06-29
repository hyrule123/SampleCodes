#include <memory>
#include <iostream>
#include <functional>


class foo
{
public:
	foo() {}
	virtual ~foo() {}

	void bar(int i)
	{
		std::cout << i;
		std::cout << " bar called!!\n";
	}
};


int main()
{
	foo b;

	std::function<void(foo*, int)> func
		= std::bind(&foo::bar, std::placeholders::_1, std::placeholders::_2);

	func(&b, 3);

	return 0;
}