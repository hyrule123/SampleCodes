# SampleCodes

유용하게 써먹을 수 있는 짧은 샘플 코드 저장소

주제: 지역변수를 이동생성자로 이동시키면 어떻게되는가?
	-> Local 변수(lvalue) 안에 있던 내용을 이동시킨 후 기존의 Local 변수는 undefined 상태가 됨(사용은 가능하나 어떻게될지 모름)
	이후 해당 변수(lvalue) 소멸 시 소멸자도 호출이 된다.