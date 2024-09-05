#include <vector>
#include <list>
#include <functional>
#include <memory>
#include <iostream>
#include <string>

std::vector<std::string> vecstr = { "1", "2", "3", "4", "5" };
std::vector<std::function<void()>> vec;

void forw() {
}

template <typename T, typename... ARGS>
void forw(T&& t, ARGS&&... args) {
	vec.push_back(
		[t]()->void {
			std::cout << t << std::endl;
		}
	);

	forw(std::forward<ARGS>(args)...);
}

int main()
{
	forw(vecstr[0], vecstr[1], vecstr[2], vecstr[3], vecstr[4]);

	vecstr.clear();

	for (const auto& f : vec) {
		f();
	}

	return 0;
}