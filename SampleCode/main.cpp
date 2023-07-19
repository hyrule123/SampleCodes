#include <string>
#include <vector>
#include <span>
#include <iostream>

int main()
{
	std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	std::span<int> sp(vec.begin() + 5, vec.end());

	
	int a = 0;
	//�ݺ��ڸ� ���۷��� ���·� ���� �� ���� �����ȴ�
	for (auto& iter : sp)
	{
		iter = a;
		++a;
	}
	//�̷������� ���� ���� ������ ������
	sp[0] = 3;

	//span�� ������ �����Ϳ� ������ ������ ��� �����Ƿ� �����Ͱ� ũ�� ����
	std::cout << "Size of span = " << sizeof(sp) << std::endl;



	//const�� �ٿ������� ������ �Ұ���
	std::span<int const> sp2(vec.begin() + 5, vec.end());
	
	//������ ����
	//sp2[3] = 3;
	for (size_t i = 0; i < vec.size(); ++i)
	{
		std::cout << vec[i] << std::endl;
	}


	//�� span�� ��ȸ���� �ƹ� �ϵ� �Ͼ�� �ʴ´�
	std::span<int> EmptySp;
	for (auto iter : EmptySp)
	{
		std::cout << iter << std::endl;
	}

	return 0;
}