#pragma once
#include "SimpleMath.h"
#include <iostream>
#include <sstream>

using namespace ::ehw::math;
class MatrixDecomposer
{
public:
	MatrixDecomposer(const Matrix& _ROW_MAJOR_homogeneous);
	~MatrixDecomposer();

	void Decompose(Matrix& T, Matrix& R, Matrix& S);

#define MATRIX_NAME(_matName) #_matName, _matName
	//MatrixCmp(MATRIX_NAME(matA), MATRIX_NAME(matB));
	void MatrixCmp(const char* _matNameA, const ::ehw::math::Matrix& _matA, const char* _matNameB, const ::ehw::math::Matrix& _matB, float _threshold = 0.01f);

	void PrintMatrix(const char* _matName, const ehw::math::Matrix& _mat);

private:
	Matrix H;
};

