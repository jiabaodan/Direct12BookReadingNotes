#include "ArrayMatrix.h"

ArrayMatrix::ArrayMatrix()
{
	m_data.fill(0);
}

ArrayMatrix::ArrayMatrix(float m00, float m01, float m02, float m03, 
	float m10, float m11, float m12, float m13, 
	float m20, float m21, float m22, float m23, 
	float m30, float m31, float m32, float m33)
{
	m_data = { m00, m01, m02, m03, 
		m10, m11, m12, m13, 
		m20, m21, m22, m23, 
		m30, m31, m32, m33 };
}

ArrayMatrix::ArrayMatrix(const ArrayMatrix& rhs)
{
	for (int index = 0; index < m_dataCount; index++)
	{
		m_data[index] = rhs.GetDataByIndex(index);
	}
}

ArrayMatrix::~ArrayMatrix()
{
}

void ArrayMatrix::TransformToTransposeMatrix()
{
	for (int indexRow = 0; indexRow < m_rowAndColumnCount; indexRow++)
	{
		for (int indexColumn = indexRow + 1; indexColumn < m_rowAndColumnCount; indexColumn++)
		{
			ChangeData(indexRow, indexColumn, indexColumn, indexRow);
		}
	}
}

float ArrayMatrix::GetDeterminant()
{
	float res = 0.f;

	// 计算3 x 3行列式
	for (int indexColumn = 0; indexColumn < m_rowAndColumnCount; indexColumn++)
	{
		int dataIndex = 0;
		array<float, 9> data3x3Temp;

		// 获取 3x3 子矩阵
		for (int indexRowSub = 1; indexRowSub < m_rowAndColumnCount; indexRowSub++)
		{
			for (int indexColumnSub = 0; indexColumnSub < m_rowAndColumnCount; indexColumnSub++)
			{
				if (indexColumnSub == indexColumn)
				{
					continue;
				}

				data3x3Temp[dataIndex++] = m_data[indexRowSub * m_rowAndColumnCount + indexColumnSub];
			}
		}

		float resTemp3x3 = m_data[indexColumn] * GetDeterminant3x3(data3x3Temp);

		if (indexColumn % 2 == 0)
		{
			res += resTemp3x3;
		}
		else
		{
			res -= resTemp3x3;
		}
		resTemp3x3 = 0.f;
	}

	return res;
}

void ArrayMatrix::TransformToAdjointMatrix()
{
	ArrayMatrix matrixTemp(*this);

	for (int indexRow = 0; indexRow < m_rowAndColumnCount; indexRow++)
	{
		for (int indexColumn = 0; indexColumn < m_rowAndColumnCount; indexColumn++)
		{
			// 获取子矩阵
			int dataIndex = 0;
			array<float, 9> data3x3Temp;

			for (int indexRowSub = 0; indexRowSub < m_rowAndColumnCount; indexRowSub++)
			{
				if (indexRowSub == indexRow)
				{
					continue;
				}

				for (int indexColumnSub = 0; indexColumnSub < m_rowAndColumnCount; indexColumnSub++)
				{
					if (indexColumnSub == indexColumn)
					{
						continue;
					}

					data3x3Temp[dataIndex++] = matrixTemp.GetData(indexRowSub, indexColumnSub);
				}
			}

			// 计算行列式
			float resTemp3x3 = GetDeterminant3x3(data3x3Temp);
			if (resTemp3x3 != 0)
			{
				resTemp3x3 = resTemp3x3 * pow(-1, indexRow + indexColumn);
			}

			this->SetData(indexRow, indexColumn, resTemp3x3);
		}
	}

	// 转换到转置矩阵
	TransformToTransposeMatrix();
}

void ArrayMatrix::TransformToInverseMatrix()
{
	float determinant = this->GetDeterminant();
	if (determinant > 0)
	{
		this->TransformToAdjointMatrix();				// 求伴随矩阵
		this->DoMultiplication(1.f / determinant);		// 求逆矩阵
	}
}

void ArrayMatrix::DoMultiplication(const ArrayMatrix &mat)
{
	ArrayMatrix matrixTemp(*this);

	for (int indexRow = 0; indexRow < m_rowAndColumnCount; indexRow++)
	{
		for (int indexColumn = 0; indexColumn < m_rowAndColumnCount; indexColumn++)
		{
			float valueTemp = 0.f;

			// 计算向量的点积
			for (int valueIndex = 0; valueIndex < m_rowAndColumnCount; valueIndex++)
			{
				valueTemp += matrixTemp.GetData(indexRow, valueIndex) * mat.GetData(valueIndex, indexColumn);
			}

			// 更新数据
			m_data[indexRow * m_rowAndColumnCount + indexColumn] = valueTemp;
		}
	}
}

void ArrayMatrix::DoMultiplication(const float scalar)
{
	for (int indexRow = 0; indexRow < m_rowAndColumnCount; indexRow++)
	{
		for (int indexColumn = 0; indexColumn < m_rowAndColumnCount; indexColumn++)
		{
			m_data[indexRow * m_rowAndColumnCount + indexColumn] = m_data[indexRow * m_rowAndColumnCount + indexColumn] * scalar;
		}
	}
}

void ArrayMatrix::ChangeData(int srcRow, int srcColumn, int desRow, int desColum)
{
	float dataTemp = m_data[srcRow * m_rowAndColumnCount + srcColumn];
	m_data[srcRow * m_rowAndColumnCount + srcColumn] = m_data[desRow * m_rowAndColumnCount + desColum];
	m_data[desRow * m_rowAndColumnCount + desColum] = dataTemp;
}

float ArrayMatrix::GetDeterminant3x3(array<float, 9> &arrayData)
{
	float resTemp3x3 = 0.f;

	// 计算 2 x 2
	for (int indexColumn = 0; indexColumn < 3; indexColumn++)
	{
		int dataIndex = 0;
		float resTemp2x2 = 0.f;
		array<float, 4> data2x2Temp;

		// 获取 2x2 子矩阵
		for (int indexRowSub = 1; indexRowSub < 3; indexRowSub++)
		{
			for (int indexColumn2x2 = 0; indexColumn2x2 < 3; indexColumn2x2++)
			{
				if (indexColumn2x2 == indexColumn)
				{
					continue;
				}

				data2x2Temp[dataIndex++] = arrayData[indexRowSub * 3 + indexColumn2x2];
			}
		}

		// 计算2x2
		resTemp2x2 = arrayData[indexColumn] * (data2x2Temp[0] * data2x2Temp[3] - data2x2Temp[1] * data2x2Temp[2]);

		if (indexColumn % 2 == 0)
		{
			resTemp3x3 += resTemp2x2;
		}
		else
		{
			resTemp3x3 -= resTemp2x2;
		}
	}

	return resTemp3x3;
}