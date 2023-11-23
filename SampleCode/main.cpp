#include <string>
#include <assert.h>

inline void AssertW(const wchar_t* _expression, const wchar_t* _message, const wchar_t* _fileName, unsigned int _line)
{
	std::wstring assertMsg = std::wstring(_expression) + std::wstring(L"\n") + std::wstring(_message);
	_wassert(assertMsg.c_str(), _fileName, _line);
}

#define ASSERT(_expression, _message) (void) (					\
	(!!(_expression)) || \
	(AssertW(L ## #_expression, L ## _message, _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \
)

#ifdef _DEBUG
#define ASSERT_DEBUG(_expression, _message) ASSERT(_expression, _message)
#else
#define ASSERT_DEBUG(_expression, _message)
#endif _DEBUG

int main(void)
{
	ASSERT(1 == 0, "1은 0이 아닙니다");

	return 0;
}