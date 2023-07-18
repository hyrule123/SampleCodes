#include <regex>
#include <string>

int main()
{
	std::string FindThread = "  [  numthreads (   128  ,  1  ,  1) ]";

	//std::regex NumThreadRegex(R"(\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*\))");
	std::regex NumThreadRegex(R"(.*\((\d+),(\d+),(\d+\)).*)");

	size_t pos = FindThread.find("numthreads");
	if (std::string::npos != pos)
	{
		FindThread.erase(std::remove_if(FindThread.begin(), FindThread.end(),
			[](char c)->bool
			{
				return std::isspace(c);
			}), FindThread.end());

		int NumThreads[3]{};
		std::smatch Matches;
		if (std::regex_match(FindThread, Matches, NumThreadRegex))
		{
			if (Matches.size() < 4)
			{
				return 0;
			}
			
			for (size_t i = 1; i < Matches.size(); ++i)
			{
				NumThreads[i - 1] = std::stoi(Matches[i]);
			}
		}


		int breakpoint = 0;
	}


	return 0;
}