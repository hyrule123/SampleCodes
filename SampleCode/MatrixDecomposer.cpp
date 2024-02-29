#include "MatrixDecomposer.h"

MatrixDecomposer::MatrixDecomposer(const Matrix& _ROW_MAJOR_homogeneous)
	: H(_ROW_MAJOR_homogeneous)
{
}

MatrixDecomposer::~MatrixDecomposer()
{
}

void MatrixDecomposer::Decompose(Matrix& T, Matrix& R, Matrix& S)
{
	T = Matrix::Identity;
	R = Matrix::Identity;
	S = Matrix::Identity;

	//Leftover
	Matrix L = Matrix::Identity;

	//Translation
	//Obtain the translation  T  from the last column and the linear component  L  from the 3x3 upper-left block.
	//->Column-Major Matrix 기준
	T.Translation(H.Translation());

	L = H;
	L.Translation(Vector3::Zero);

	MatrixCmp(MATRIX_NAME(H), MATRIX_NAME(L * T));


	//Rotation
	//Calculate the polar decomposition of  L  to obtain rotation  R and stretch  K  matrices :
	//L=RK 
	//H = TRK
	Matrix K{};

	//R = EigenVector
}


void MatrixDecomposer::MatrixCmp(const char* _matNameA, const::ehw::math::Matrix& _matA, const char* _matNameB, const::ehw::math::Matrix& _matB, float _threshold)
{
	std::cout << "\n\n\n============================================" << std::endl;
	PrintMatrix(_matNameA, _matA);
	PrintMatrix(_matNameB, _matB);

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

void MatrixDecomposer::PrintMatrix(const char* _matName, const ehw::math::Matrix& _mat)
{
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

	std::cout << "============================================\n\n\n" << std::endl;
}
