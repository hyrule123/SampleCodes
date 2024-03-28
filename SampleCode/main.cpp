#include <vector>
#include <list>
#include <functional>
#include <memory>
#include <iostream>

void foo(std::unique_ptr<int>&& i) noexcept
{
	if (i)
	{
		std::cout << *i << std::endl;
	}
}

int main()
{
	std::vector<int> vec1{ 1, 2, 3, 4, 5 };
	std::vector<int> vec2{ 5, 6 ,7 ,8 ,9 };

	//from 벡터의 사이즈를 알 수 있는 경우 O(n)이라고 한다.
	auto iter = vec1.insert(vec1.end(), std::make_move_iterator(vec2.begin() + 1), std::make_move_iterator(vec2.end()));

	//iter 반환값: 옮겨붙인 곳의 시작점 iterator
	std::cout << *iter << std::endl;

	//옮기고 난 뒤에 vec2는 undefined state가 되므로 clear 또는 옮겨진 부분 뒷쪽을 제거해주어야 한다.
	vec2.resize((vec2.begin() + 1) - vec2.begin());

	return 0;
}