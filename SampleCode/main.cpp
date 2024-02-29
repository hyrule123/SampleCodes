#include "SimpleMath.h"

#include <iostream>
#include <sstream>

void PrintMatrix(const char* _matName, const ehw::math::Matrix& _mat)
{
	using namespace ::ehw::math;

	std::cout << "\n행렬 이름: " << _matName << std::endl;
	
	for (int i = 0; i < 4; ++i)
	{
		std::stringstream str{};

		for (int j = 0; j < 4; ++j)
		{
			str << std::to_string(_mat.m[i][j]) << ", ";
		}

		str << "\n";

		std::cout << str.str() << std::endl;
	}
}

void MatrixCmp(const ::ehw::math::Matrix& _matA, const ::ehw::math::Matrix& _matB, float _threshold = 0.01f)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (_threshold < fabsf(_matA.m[i][j] - _matB.m[i][j]))
			{
				std::cout << "=========================";
				std::cout << " 다름 ";
				std::cout << "=========================" << std::endl;
				return;
			}
		}
	}

	std::cout << "=========================";
	std::cout << " 같음 ";
	std::cout << "=========================" << std::endl;
}

int main()
{
	using namespace ::ehw::math;
	Matrix child = Matrix::Identity;
	Matrix parent = Matrix::Identity;

	Vector3 parentScale = Vector3(1.f, 2.f, 3.f);
	Quaternion parentRot = Quaternion::CreateFromYawPitchRoll(Vector3(XM_PI / 1.f, XM_PI / 2.f, XM_PI / 3.f));
	Vector3 parentPos = Vector3(3.f, 2.f, 1.f);
		
	parent *= Matrix::CreateScale(parentScale);
	parent *= Matrix::CreateFromQuaternion(parentRot);
	parent *= Matrix::CreateTranslation(parentPos);

	Vector3 childScale = Vector3(4.f, 3.f, 2.f);
	Quaternion childRot = Quaternion::CreateFromYawPitchRoll(Vector3(XM_PI * 3.1235f, XM_PI * 12.3853f, XM_PI * 8.192f));
	Vector3 childPos = Vector3(4.f, 8.f, 12.f);
	child *= Matrix::CreateScale(childScale);
	child *= Matrix::CreateFromQuaternion(childRot);
	child *= Matrix::CreateTranslation(childPos);

	Matrix childWorld = child * parent;

	Vector3 childWorldScale = childScale * parentScale;
	Quaternion childWorldRot = childRot * parentRot;
	Vector3 chileWorldPos = childWorld.Translation();

	//행렬끼리 곱한 결과(SS * RR * Tw)와 world SRT를 각각 구한 값이 동일한지 확인
	//결론: 크기 행렬이 X, Y, Z 모두 동일 배율일 경우 동일하지만, 
	// X, Y, Z 모두 다른 배율일 경우 동일하지 않음.
	Matrix createFromWorldValues = Matrix::CreateScale(childWorldScale)
		* Matrix::CreateFromQuaternion(childWorldRot)
		* Matrix::CreateTranslation(chileWorldPos);

	Matrix scaleMat = Matrix::CreateScale(childWorldScale);

	PrintMatrix("childWorld", childWorld);
	PrintMatrix("createFromWorldValues", createFromWorldValues);

	MatrixCmp(childWorld, createFromWorldValues);
	//========================================================================


	//Scale 없이 만든 Matrix와 Scale 포함해서 만든 Matrix에  1/ scale matrix를 곱한 결과가 같은지?
	//결론: 안됨. Scale이 XYZ 모두 동일한 배율이면 되는데 그렇지 않을경우 안됨.
	Matrix childWorldScaleInv = Matrix::CreateScale(Vector3(1.f) / childWorldScale);
	Matrix childWorldScaleInversed = childWorld * childWorldScaleInv;
	//childWorldWithoutTranslation.Translation(Vector3(0.f, 0.f, 0.f));


	Matrix createWithoutScale = Matrix::CreateScale(1.f)
		* Matrix::CreateFromQuaternion(childWorldRot)
		* Matrix::CreateTranslation(childWorld.Translation());

	
	
	PrintMatrix("childWorldScaleInversed", childWorldScaleInversed);
	PrintMatrix("createWithoutScale", createWithoutScale);

	MatrixCmp(childWorldScaleInversed, createWithoutScale);
	//======================================================================================

	//Scale 없이 만든 Matrix와 World Matrix를 Normalize한 결과는 같은가?
	createWithoutScale;
	Matrix normalizedWorldMat = childWorld;
	normalizedWorldMat.Up(normalizedWorldMat.Up().Normalize());
	normalizedWorldMat.Right(normalizedWorldMat.Right().Normalize());
	normalizedWorldMat.Forward(normalizedWorldMat.Forward().Normalize());

	PrintMatrix("createWithoutScale", createWithoutScale);
	PrintMatrix("normalizedWorldMat", normalizedWorldMat);

	MatrixCmp(createWithoutScale, normalizedWorldMat);
	//======================================================================================


	Matrix childWorld_ScaleNormalized = childWorld;
	childWorld_ScaleNormalized.Right(childWorld_ScaleNormalized.Right().Normalize());
	childWorld_ScaleNormalized.Up(childWorld_ScaleNormalized.Up().Normalize());
	childWorld_ScaleNormalized.Forward(childWorld_ScaleNormalized.Forward().Normalize());

	PrintMatrix("childWorldScaleInversed", childWorldScaleInversed);
	PrintMatrix("childWorld_ScaleNormalized", childWorld_ScaleNormalized);

	MatrixCmp(childWorldScaleInversed, childWorld_ScaleNormalized);

	//======================================================================================

	//내장된 Decompose 함수를 통한 SRT 분해
	Vector3 decomposedWorldScale{};
	Quaternion decomposedWorldRot{};
	Vector3 decomposedWorldPos{};

	childWorld.Decompose(decomposedWorldScale, decomposedWorldRot, decomposedWorldPos);


	//쿼터니언 역수 확인
	
	Matrix invWorldRot = Matrix::CreateFromQuaternion(childWorldRot);
	Matrix childWorld_multiplyInvRot = invWorldRot * childWorld;
	
	Matrix childWorld_recomposed_withoutRot = 
		Matrix::CreateScale(decomposedWorldScale)
		* Matrix::CreateFromQuaternion(Quaternion::Identity)
		* Matrix::CreateTranslation(decomposedWorldPos);

	PrintMatrix("childWorld_multiplyInvRot", childWorld_multiplyInvRot);
	PrintMatrix("childWorld_recomposed_withoutRot", childWorld_recomposed_withoutRot);
	MatrixCmp(childWorld_multiplyInvRot, childWorld_recomposed_withoutRot);


	//Scale만 곱한 값의 역수를 앞에 곱해주면 크기정보 제거가 가능한지 확인
	Matrix recomposedChild_without_scale = 
		Matrix::CreateScale(Vector3::One) 
		* Matrix::CreateFromQuaternion(decomposedWorldRot) 
		* Matrix::CreateTranslation(decomposedWorldPos);

	Vector3 childWorldScaleInvert = Vector3(1.f) / (childScale * parentScale);
	Matrix childWorld_without_scale = Matrix::CreateScale(childWorldScaleInvert) * childWorld;

	PrintMatrix("recomposedChild_without_scale", recomposedChild_without_scale);
	PrintMatrix("childWorld_without_scale", childWorld_without_scale);

	MatrixCmp(recomposedChild_without_scale, childWorld_without_scale);

	return 0;
}



