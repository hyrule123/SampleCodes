﻿#include <string>

//참고
//https://stackoverflow.com/questions/9356169/utf-8-continuation-bytes
size_t Strlen_UTF8(const std::string_view _str);

int main()
{
	std::string test = "안녕하세요.";

	size_t len = Strlen_UTF8(test);

	return 0;

}


size_t Strlen_UTF8(const std::string_view _str)
{
	//UTF-8 길이에 따른 비트 값
	//1 byte:  0b 0xxx xxxx
	//2 bytes: 0b 110x xxxx / 01xx xxxx
	//3 bytes: 0b 1110 xxxx / 01xx xxxx / 01xx xxxx
	//4 bytes: 0b 1111 0xxx / 01xx xxxx / 01xx xxxx / 01xx xxxx
	//이론상 이런식으로 6바이트까지 가능하나 호환성을 위해 쓰지 않음.
	
	//연속된 바이트를 사용할 떄 첫 바이트는 '11'로 시작하는 것을 알수 있다.
	//->만약 11로 시작하는 비트일 때, '11'에서 앞의 1('10')을 제거해준 다음, 여전히 참이면 카운트를 올려주면 된다.
	
	constexpr char leadingByte = 0b11000000;
	constexpr char ContinuationByte = 0b10000000;
	size_t len{};
	for (size_t i = 0; i < _str.size(); ++i)
	{
		//Continuation Byte와 한번 더 체크를 하는 이유: '11'과 & 연산을 시행했을 때 '01'만 남을수도 있음
		//bool 연산 결과를 바로 +=로 넘겨준다.
		len += ((_str[i] & leadingByte) != ContinuationByte);
	}
	return len;
}
