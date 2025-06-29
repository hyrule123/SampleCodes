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
	std::shared_ptr<foo> sptr = std::make_shared<foo>();

	std::function<void(const std::shared_ptr<foo>&, int)> func
		= std::bind(&foo::bar, std::placeholders::_1, std::placeholders::_2);

	func(sptr, 3);

	return 0;
}