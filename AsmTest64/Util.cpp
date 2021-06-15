#include "pch.h"
#include "math.inl"
#include "Util.h"


void SetIdentityMatrix(MATRIX4* pOutMat)
{
	pOutMat->_12 = pOutMat->_13 = pOutMat->_14 = pOutMat->_21 = pOutMat->_23 = pOutMat->_24 = 0.0f;
	pOutMat->_31 = pOutMat->_32 = pOutMat->_34 = pOutMat->_41 = pOutMat->_42 = pOutMat->_43 = 0.0f;
	pOutMat->_11 = pOutMat->_22 = pOutMat->_33 = pOutMat->_44 = 1.0f;
}

void FillMatrix(MATRIX4* pOutMat)
{
	SetIdentityMatrix(pOutMat);

	for (DWORD y = 0; y < 4; y++)
	{
		for (DWORD x = 0; x < 4; x++)
		{
			pOutMat->f[y][x] = (float)((rand() % 10) + 1) / 10.0f;
		}
	}
}
void FillVector4(VECTOR4* pv3Out)
{
	float* pf = &pv3Out->x;
	for (DWORD i = 0; i < 4; i++)
	{
		pf[i] = (float)((rand() % 10) + 1) / 10.0f;
	}
}
void PrintMatrix(MATRIX4* pMat)
{
	for (DWORD y = 0; y < 4; y++)
	{
		wprintf_s(L"%.2f %.2f %.2f %.2f\n", pMat->f[y][0], pMat->f[y][1], pMat->f[y][2], pMat->f[y][3]);
	}
}