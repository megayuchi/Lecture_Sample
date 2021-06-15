#pragma once

struct VECTOR4;
struct MATRIX4;
extern "C" 
{
	void TransformVector4_VPTR2_ASM(VECTOR4* pv4Dest, const VECTOR4* pv4Src, const MATRIX4* pMat, unsigned int uiCount);
}
