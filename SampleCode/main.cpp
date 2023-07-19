#include <string>
#include <vector>
#include <span>
#include <iostream>

int main()
{
	std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	std::span<int> sp(vec.begin() + 5, vec.end());

	
	int a = 0;
	//반복자를 레퍼런스 형태로 받을 시 값이 수정된다
	for (auto& iter : sp)
	{
		iter = a;
		++a;
	}
	//이런식으로 직접 접근 수정도 가능함
	sp[0] = 3;

	//span은 시작점 포인터와 데이터 갯수만 들고 있으므로 데이터가 크지 않음
	std::cout << "Size of span = " << sizeof(sp) << std::endl;



	//const를 붙여버리면 수정이 불가능
	std::span<int const> sp2(vec.begin() + 5, vec.end());
	
	//컴파일 에러
	//sp2[3] = 3;
	for (size_t i = 0; i < vec.size(); ++i)
	{
		std::cout << vec[i] << std::endl;
	}


	//빈 span을 순회돌면 아무 일도 일어나지 않는다
	std::span<int> EmptySp;
	for (auto iter : EmptySp)
	{
		std::cout << iter << std::endl;
	}

	return 0;
}