#include <iostream>
#include <vector>
#include <string>

struct Child
{
public:
	Child() : m_id(++s_id) { ID(); std::cout << "Child 생성자 호출" << std::endl; };
	Child(const Child& _other) : m_id(++s_id) { ID(); std::cout << "Child 복사 생성자 호출" << std::endl; }
	Child(Child&& _move) noexcept : m_id(std::move(_move.m_id)) { ID(); std::cout << "Child 이동 생성자 호출" << std::endl; }
	~Child() { ID(); std::cout << "Child 소멸자 호출" << std::endl; }

	Child& operator=(const Child& _other) { m_id = _other.m_id; std::cout << "Child 대입 연산자 호출" << std::endl; return *this; }
	Child& operator=(Child&& _other) noexcept { m_id = std::move(_other.m_id); std::cout << "Child 이동 연산자 호출" << std::endl; return *this; }

private:
	int m_id;
	void ID() { std::cout << "ID: " << m_id << " - "; }

	static int s_id;
	
};
int Child::s_id = 0;

class Parent
{
public:
	Parent() : m_var() { std::cout << "Parent 생성자 호출" << std::endl; }

	Parent(const Parent& _other) : m_var(_other.m_var) { std::cout << "Parent 복사 생성자 호출" << std::endl; }

	Parent(Parent&& _move) noexcept : m_var(std::move(_move.m_var)) { std::cout << "Parent 이동 생성자 호출" << std::endl; }

	~Parent() { std::cout << "Parent 소멸자 호출" << std::endl; }

	Parent& operator=(const Parent& _other) { m_var = _other.m_var; std::cout << "Parent 대입 연산자 호출" << std::endl; return *this; }
	Parent& operator=(Parent&& _other) noexcept { m_var = std::move(_other.m_var); std::cout << "Parent 이동 연산자 호출" << std::endl; return *this; }

private:
	Child m_var;
};



int main()
{
	std::vector<Parent> vecTst(0);
	
	{
		Parent tst{};
		std::cout << std::endl;

		std::cout << "<push_back>" << std::endl;
		vecTst.push_back(tst);
		std::cout << std::endl;

		std::cout << "<Reserve std::vector size to 10>" << std::endl;
		vecTst.reserve(10);
		std::cout << std::endl;

		//복사생성해서 새로운 인스턴스를 생성 후 해당 인스턴스를 이동시킨다.
		std::cout << "<std::move 없는 emplace_back>" << std::endl;
		vecTst.emplace_back(tst);
		std::cout << std::endl;

		std::cout << "<std::move 사용한 emplace_back>" << std::endl;
		vecTst.emplace_back(std::move(tst));
		//이걸 주석풀고 한번 더 해주면 경고가 나오는걸 볼수있음. 개체에서 이동됨 사용:tst (lifetime 1)
		//vecTst.emplace_back(std::move(tst));
		std::cout << std::endl;

		//https://stackoverflow.com/questions/55376494/default-move-assignment-calls-destructor-copy-assignment-doesnt
		//Move할 경우 소멸자가 호출된다고 함.(기존 tst에 있던 undefined 상태의 변수에 대한 소멸자 호출인듯)
		std::cout << "<undefined 상태의 tst lvalue에 새 변수 할당>" << std::endl;
		tst = Parent();
		std::cout << std::endl;

		//emplace_back과 똑같이 이동생성자가 호출되고 있음.
		std::cout << "<std::move 사용한 push_back>" << std::endl;
		vecTst.push_back(std::move(tst));
		std::cout << std::endl;



		int a = 3;
	}
}

