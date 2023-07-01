
#include <filesystem>
#include <type_traits>
#include <string>
#include <fstream>
#include <Windows.h>


namespace stdfs = std::filesystem;

template <typename T>
class CCodeWriter
{
public:
	CCodeWriter() {};
	~CCodeWriter() {};

private:

	//SFINAE overload to char type
	template <typename U = T, typename std::enable_if_t<std::is_same_v<U, wchar_t>, int> = 0>
	const std::basic_string_view<U> ConvString(const std::string_view _AstrView, const std::wstring_view _WstrView)
	{
		return _WstrView;
	}
	
	//SFINAE overload to wchar_t type
	template <typename U = T, typename std::enable_if_t<std::is_same_v<U, char>, int> = 0>
	const std::basic_string_view<U> ConvString(const std::string_view _AstrView, const std::wstring_view _WstrView)
	{
		return _AstrView;
	}

#define CONV_STRING(str) ConvString(str, L##str)


private:
	int m_iIndentation;
	std::basic_ofstream<T> m_ofs;

public:
	HRESULT Open(stdfs::path const& _path) { return S_OK; }
	bool IsOpen() const { return m_ofs.is_open(); }

	void WriteCode(const std::basic_string_view<T> _strCode)
	{
		//...

		std::string_view newstrv = CONV_STRING("\t");

		//...
	}
};


int main()
{
	CCodeWriter<char> writer;

	writer.WriteCode("HI");

	return 0;
}