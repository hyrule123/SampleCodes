#include "MatrixDecomposer.h"

int main()
{
	using namespace ::ehw::math;

	//정답
	Matrix TransformationOriginal =
	{
		-2.f, -1.f, 0.f, 2.f,
		-2.f, 1.f, 0.f, -1.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	//질문자가 잘못 입력한 Scale 값을 집어넣었을 때 나오는 결과

	Matrix Transformation = Transformation.Transpose();

	Matrix Translation =
	{
		1.f, 0.f, 0.f, 2.f,
		0.f, 1.f, 0.f, -1.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};
	Translation = Translation.Transpose();

	float sqrt2Inv = 1.f / sqrtf(2.f);
	Matrix Rotation =
	{
		sqrt2Inv,	-sqrt2Inv,	0.f,	0.f,
		sqrt2Inv,	sqrt2Inv,	0.f,	0.f,
		0.f,		0.f,		1.f,	0.f,
		0.f,		0.f,		0.f,	1.f
	};
	Rotation = Rotation.Transpose();

	float sqrt2 = sqrtf(2.f);

	//원래 맞는 Scale 행렬
	Matrix ScaleOriginal = 
	{
		-2.f * sqrt2,	0.f,	0.f,	0.f,
		0.f,			sqrt2,	0.f,	0.f,
		0.f,			0.f,	1.f,	0.f,
		0.f,			0.f,	0.f,	1.f
	};

	Matrix Scale = ScaleOriginal.Transpose();

	Matrix World = Scale * Rotation * Translation;

	
	Matrix H = {
		1.f, 0.f, 1.f, 0.f,
		1.f, 1.f, 1.f, 0.f,
		1.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 0.f, 1.f
	};
	H = H.Transpose();
	MatrixDecomposer decomposer(H);
	
	Matrix T, R, S;
	decomposer.Decompose(T, R, S);


	//Translation 구하기
	//Obtain the translation  T  from the last column and the linear component  L  from the 3x3 upper-left block.
	//H = TL



	return 0;

}



