# SampleCodes

유용하게 써먹을 수 있는 짧은 샘플 코드 저장소

* NRVO(Named Return Value Optimization)
	- 반환하는 변수가 non-volatile일 때 적용된다고 함.
	- 반환받을 l-value에 직접 반환될 데이터를 작성
	- 상황에 따라 반환값이 달라지는 경우에는 적용이 되지 않음

	
* 분기가 있는 함수에 NRVO 복사 생략이 적용되는가?
* 참고 코드: https://stackoverflow.com/questions/35736568/is-there-a-way-to-check-if-rvo-was-applied
* 결론: 적용됨


NRVO가 적용되었을 때(분기에 상관없이 하나의 지역변수를 반환할 때)
Created!!
Created!!
Local Value 1 address: 0000002EAC79F734
Local Value 2 address: 0000002EAC79F5E4
REMOVED!!
Returned Value address: 0000002EAC79F734
-> Local Value 1의 주소와 반환받은 변수의 주소가 같은것을 확인 가능하다.

NVRO가 적용되지 않았을 때(분기에 따라 반환하는 변수가 바뀔 때)
Created!!
Created!!
Local Value 1 address: 0000008D6BAFF3D4
Local Value 2 address: 0000008D6BAFF3F4
MOVED!!
REMOVED!!
REMOVED!!
Returned Value address: 0000008D6BAFF544
-> 두 단계가 추가된 것을 확인 가능. 또한 반환받은 변수의 주소가 달라졌다.

*참고 주소: https://hodongman.github.io/2020/11/02/C++-copy-Elision-copy.html