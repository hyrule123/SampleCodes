#include <filesystem>
#include <iostream>

namespace std
{
	namespace fs = filesystem;
}

int main()
{
	std::fs::path add;
	add /= "x64";
	std::fs::current_path(add);
	
	std::string newpath = std::fs::current_path().string();
	std::cout << newpath << std::endl;

	std::fs::path relPath = ".";
	std::fs::path absPath = std::fs::absolute(relPath);
	std::cout << absPath.string() << std::endl;

	bool b = std::filesystem::exists("asdf/asdfasdf/");



	return 0;
}