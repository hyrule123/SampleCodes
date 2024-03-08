# SampleCodes

유용하게 써먹을 수 있는 짧은 샘플 코드 저장소

	//참고
	//https://stackoverflow.com/questions/9356169/utf-8-continuation-bytes

	//UTF-8 길이에 따른 비트 값
	//1 byte:  0b 0xxx xxxx
	//2 bytes: 0b 110x xxxx / 01xx xxxx
	//3 bytes: 0b 1110 xxxx / 01xx xxxx / 01xx xxxx
	//4 bytes: 0b 1111 0xxx / 01xx xxxx / 01xx xxxx / 01xx xxxx
	//이론상 이런식으로 6바이트까지 가능하나 호환성을 위해 쓰지 않음.
	
	//연속된 바이트를 사용할 떄 첫 바이트는 '11'로 시작하는 것을 알수 있다.
	//->만약 11로 시작하는 비트일 때, '11'에서 앞의 1('10')을 제거해준 다음, 여전히 참이면 카운트를 올려주면 된다.