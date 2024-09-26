#include <chrono>
#include <vector>
#include <iostream>

struct matrix {
	double m[16];
};

constexpr size_t testcount = 100000;

__declspec(noinline) void test1() {
	std::vector<matrix> vec;
	vec.reserve(testcount);

	size_t elemval = 0;
	for (size_t i = 0; i < testcount; ++i) {
		matrix mat;
		for (size_t j = 0; j < 16; ++j) {
			mat.m[j] = (double)++elemval;
		}
		vec.push_back(mat);
	}
}
__declspec(noinline) void test2() {
	std::vector<matrix> vec;
	vec.resize(testcount);

	size_t elemval = 0;
	for (size_t i = 0; i < testcount; ++i) {
		for (size_t j = 0; j < 16; ++j) {
			vec[i].m[j] = (double)++elemval;
		}
	}
}

int main() {
	std::chrono::steady_clock::time_point start;
	std::chrono::duration<float> dur;

	start = std::chrono::steady_clock::now();
	test1();
	dur = std::chrono::steady_clock::now() - start;
	std::cout << dur.count() << std::endl;

	start = std::chrono::steady_clock::now();
	test2();
	dur = std::chrono::steady_clock::now() - start;
	std::cout << dur.count() << std::endl;

	return 0;
}