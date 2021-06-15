#pragma once

struct VECTOR4;
struct MATRIX4;
extern "C" {
	INT64	Add_ASM(INT64 a, INT64 b);
	INT64	Add_ASM_RBP(INT64 a, INT64 b);
	INT64	FillString_ASM(char* pDest, INT64 Count);
	unsigned int GetMaxValue_ASM64_CMOV(const unsigned int* puiValueList, unsigned int uiCount);
	unsigned int GetMaxValue_ASM64_XOR(const unsigned int* puiValueList, unsigned int uiCount);
}

INT64 Add_C(INT64 a, INT64 b);
