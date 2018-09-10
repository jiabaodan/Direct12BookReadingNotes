#include <windows.h> // for XMVerifyCPUSupport
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>

#include "ArrayMatrix.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

ostream& XM_CALLCONV operator << (ostream& os, ArrayMatrix m)
{
	for (int index = 1; index <= m.GetDataCount(); ++index)
	{
		os << m.GetDataByIndex(index - 1) << "\t";

		if (index % 4 == 0)
		{
			os << endl;
		}
	}
	return os;
}

int main()
{
	// Check support for SSE2 (Pentium4, AMD K8, and above).
	if (!XMVerifyCPUSupport())
	{
		cout << "directx math not supported" << endl;
		return 0;
	}

	ArrayMatrix matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 4.0f, 0.0f, 
		1.0f, 2.0f, 3.0f, 1.0f);

	cout << "Source matrix: " << endl << matrix << endl;

	// 转置矩阵
	ArrayMatrix transposeMatrix(matrix);
	transposeMatrix.TransformToTransposeMatrix();
	cout << "After transform to transpose: " << endl << transposeMatrix << endl;

	// 矩阵的行列式
	float delterminant = matrix.GetDeterminant();
	cout << "Determinant is: " << delterminant << endl << endl;
	if (delterminant == 0)
	{
		cout << "Determinant is zero, this matrix have no inverse matrix!" << endl;

		system("pause");
		return 0;
	}

	// 伴随矩阵
	ArrayMatrix adjointOfMatrix(matrix);
	adjointOfMatrix.TransformToAdjointMatrix();
	cout << "After transform to adjoint of matrix: " << endl << adjointOfMatrix << endl;

	// 逆矩阵
	ArrayMatrix inverseMatrix(matrix);
	inverseMatrix.TransformToInverseMatrix();
	cout << "After transform inverse: " << endl << inverseMatrix << endl;

	// 计算结果的验证
	matrix.DoMultiplication(inverseMatrix);
	cout << "After do multiplication with inverse matrix: " << endl << matrix << endl;

	system("pause");
	return 1;
}