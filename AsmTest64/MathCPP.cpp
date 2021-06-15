#include "pch.h"
#include "math.inl"
#include "MathCPP.h"

void TransformVector4_VPTR2_C_SSE(VECTOR4* pv4Dest, const VECTOR4* pv4Src, const MATRIX4* pMat, unsigned int uiCount)
{
	__m128 m0 = _mm_loadu_ps((float*)&pMat->_11);
	__m128 m1 = _mm_loadu_ps((float*)&pMat->_21);
	__m128 m2 = _mm_loadu_ps((float*)&pMat->_31);
	__m128 m3 = _mm_loadu_ps((float*)&pMat->_41);

	for (unsigned int i = 0; i < uiCount; i++)
	{
		__m128 src = _mm_loadu_ps((float*)pv4Src);

		__m128 xxxx = _mm_shuffle_ps(src, src, 0b00000000);
		__m128 yyyy = _mm_shuffle_ps(src, src, 0b01010101);
		__m128 zzzz = _mm_shuffle_ps(src, src, 0b10101010);
		__m128 wwww = _mm_shuffle_ps(src, src, 0b11111111);

		__m128 r0 = _mm_mul_ps(xxxx, m0);
		__m128 r1 = _mm_mul_ps(yyyy, m1);
		__m128 r2 = _mm_mul_ps(zzzz, m2);
		__m128 r3 = _mm_mul_ps(wwww, m3);

		r0 = _mm_add_ps(r0, r1);
		r2 = _mm_add_ps(r2, r3);
		r0 = _mm_add_ps(r0, r2);

		_mm_storeu_ps((float*)pv4Dest, r0);

		pv4Src++;
		pv4Dest++;
	}
}

void TransformVector4_VPTR2_C(VECTOR4* pv4Dest, const VECTOR4* pv4Src, const MATRIX4* pMat, unsigned int uiCount)
{
	for (unsigned int i = 0; i < uiCount; i++)
	{
		pv4Dest->x = pv4Src->x * pMat->_11 + pv4Src->y * pMat->_21 + pv4Src->z * pMat->_31 + pv4Src->w * pMat->_41;
		pv4Dest->y = pv4Src->x * pMat->_12 + pv4Src->y * pMat->_22 + pv4Src->z * pMat->_32 + pv4Src->w * pMat->_42;
		pv4Dest->z = pv4Src->x * pMat->_13 + pv4Src->y * pMat->_23 + pv4Src->z * pMat->_33 + pv4Src->w * pMat->_43;
		pv4Dest->w = pv4Src->x * pMat->_14 + pv4Src->y * pMat->_24 + pv4Src->z * pMat->_34 + pv4Src->w * pMat->_44;

		pv4Src++;
		pv4Dest++;
	}
}