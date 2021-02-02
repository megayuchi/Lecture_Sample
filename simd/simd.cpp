// simd.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <stdio.h>
#include <intrin.h>
#include <conio.h>

void TestSIMD_Load_Save();

void TestSIMD_Add();
void TestSIMD_Sub();
void TestSIMD_Mul();
void TestSIMD_Div();
void TestSIMD_Rcp();
void TestSIMD_Sqrt();

void TestSIMD_Float4ToInt4();
void TestSIMD_Int4ToFloat4();
void TestSIMD_FloatToInt();
void TestSIMD_IntToFloat();
void TestSIMD_CMP();
void Test_HADD();

int main()
{

	TestSIMD_Load_Save();

	TestSIMD_Add();
	TestSIMD_Sub();
	TestSIMD_Mul();
	TestSIMD_Div();
	TestSIMD_Rcp();
	TestSIMD_Sqrt();

	TestSIMD_Float4ToInt4();
	TestSIMD_Int4ToFloat4();
	TestSIMD_FloatToInt();
	TestSIMD_IntToFloat();

	TestSIMD_CMP();
	
	Test_HADD();

	_getch();
}

void Test_HADD()
{
	__m128 a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
	//__m128 a = _mm_setr_ps(1.0f, 2.0f, 3.0f, 4.0f);
	
	// C
	__m128 b = _mm_hadd_ps(a, a);
	b = _mm_hadd_ps(b, b);

	// ASM
	__asm
	{
		
		movaps xmm0, xmmword ptr[a] ; 4  |  3 |  2 |  1 |
		haddps xmm0,xmm0			; 7  |  3 |  7 |  3 |
		haddps xmm0,xmm0			; 10 | 10 | 10 | 10 |
		movaps xmmword ptr[b],xmm0
	}

	wprintf_s(L"%.2f, %.2f, %.2f, %.2f\n", b.m128_f32[0], b.m128_f32[1], b.m128_f32[2], b.m128_f32[3]);

}
void TestSIMD_CMP()
{
	// a > b ? a = b
	__m128 a = { 1.0f, 2.0f, 3.0f, 4.0f };
	__m128 b = { 1.0f, 6.0f, 2.0f, 8.0f };
	__m128 xor_mask = _mm_xor_ps(a, b);
	__m128 and_mask = _mm_cmpgt_ps(a, b);
	xor_mask = _mm_and_ps(xor_mask, and_mask);
	a = _mm_xor_ps(a, xor_mask);

	if (a.m128_f32[0] < b.m128_f32[0])
	{
		int a = 0;
	}
	else
	{
		int b = 0;
	}
	if (_mm_comige_ss(a, b))
	{
		int a = 0;
	}
	else
	{
		int b = 0;
	}
}
void TestSIMD_Load_Save()
{
	__declspec(align(16)) float a[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
	__declspec(align(16)) float b[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	__m128 x0 = _mm_load_ps(a);
	__m128 x1 = x0;
	_mm_store_ps(b, x1);

	wprintf_s(L"%.2f, %.2f, %.2f, %.2f\n", b[0], b[1], b[2], b[3]);
}

void TestSIMD_Add()
{
	__m128 a = { 1.0f, 2.0f, 3.0f, 4.0f };
	__m128 b = { 5.0f, 6.0f, 7.0f, 8.0f };
	__m128 c = _mm_add_ps(a, b);

	wprintf_s(L"%.2f, %.2f, %.2f, %.2f\n", c.m128_f32[0], c.m128_f32[1], c.m128_f32[2], c.m128_f32[3]);
}
void TestSIMD_Sub()
{
	__m128 a = { 1.0f, 2.0f, 3.0f, 4.0f };
	__m128 b = { 5.0f, 6.0f, 7.0f, 8.0f };
	__m128 c = _mm_sub_ps(a, b);

	wprintf_s(L"%.2f, %.2f, %.2f, %.2f\n", c.m128_f32[0], c.m128_f32[1], c.m128_f32[2], c.m128_f32[3]);
}
void TestSIMD_Mul()
{
	__m128 a = { 1.0f, 2.0f, 3.0f, 4.0f };
	__m128 b = { 5.0f, 6.0f, 7.0f, 8.0f };
	__m128 c = _mm_mul_ps(a, b);

	wprintf_s(L"%.2f, %.2f, %.2f, %.2f\n", c.m128_f32[0], c.m128_f32[1], c.m128_f32[2], c.m128_f32[3]);
}
void TestSIMD_Div()
{
	__m128 a = { 1.0f, 2.0f, 3.0f, 4.0f };
	__m128 b = { 5.0f, 6.0f, 7.0f, 8.0f };
	__m128 c = _mm_div_ps(a, b);

	wprintf_s(L"%.2f, %.2f, %.2f, %.2f\n", c.m128_f32[0], c.m128_f32[1], c.m128_f32[2], c.m128_f32[3]);
}
void TestSIMD_Rcp()
{
	__m128 a = { 1.0f, 2.0f, 3.0f, 4.0f };
	__m128 c = _mm_rcp_ps(a);

	wprintf_s(L"%.2f, %.2f, %.2f, %.2f\n", c.m128_f32[0], c.m128_f32[1], c.m128_f32[2], c.m128_f32[3]);
}
void TestSIMD_Sqrt()
{
	__m128 a = { 1.0f, 2.0f, 3.0f, 4.0f };
	__m128 c = _mm_sqrt_ps(a);

	wprintf_s(L"%.2f, %.2f, %.2f, %.2f\n", c.m128_f32[0], c.m128_f32[1], c.m128_f32[2], c.m128_f32[3]);
}
void TestSIMD_Float4ToInt4()
{
	//
	// 32bits float 4價Ы -> 32綠お int 4價Ы
	//

	// C
	__m128 a = { 1.0f, 2.0f, 3.0f, 4.0f };
	__m128i b = _mm_cvttps_epi32(a);
		
	// ASM
	__asm
	{
		movaps xmm0, xmmword ptr[a]
		cvttps2dq xmm1, xmm0
		movaps xmmword ptr[b], xmm1
	}
	wprintf_s(L"%d, %d, %d, %d\n", b.m128i_i32[0], b.m128i_i32[1], b.m128i_i32[2], b.m128i_i32[3]);
}
void TestSIMD_Int4ToFloat4()
{
	//
	// 32 bits int 4價Ы -> 32bits float 4價Ы
	//

	// C
	__m128i a = _mm_set_epi32(4, 3, 2, 1);
	__m128 b = _mm_cvtepi32_ps(a);   
	
	// ASM
	__asm
	{
		movaps xmm0, xmmword ptr[a]
		cvtdq2ps xmm1, xmm0
		movaps xmmword ptr[b], xmm1
	}
	wprintf_s(L"%.2f, %.2f, %.2f, %.2f\n", b.m128_f32[0], b.m128_f32[1], b.m128_f32[2], b.m128_f32[3]);
}
void TestSIMD_FloatToInt()
{
	//
	// 32 bits float 1價Ы -> 32 bits int 1價Ы
	//

	// C
	__m128 a = _mm_set1_ps(1.0f);
	int b = _mm_cvttss_si32(a);

	// ASM
	__asm
	{
		movaps xmm0, xmmword ptr[a]
		cvttss2si   eax, xmm0
		mov dword ptr[b],eax
	}
	wprintf_s(L"%d\n", b);

}
void TestSIMD_IntToFloat()
{
	//
	// 32 bits int 1價Ы -> 32 bits float 1價Ы
	//

	// C
	__m128 a = _mm_set1_ps(0.0f);
	int b = 16;
	__m128 c = _mm_cvtsi32_ss(a, b);
	
	// ASM
	__asm
	{
		xorps	xmm0,xmm0
		mov eax,dword ptr[b]
		cvtsi2ss    xmm0, eax
		movaps xmmword ptr[c], xmm0
	}
	
	wprintf_s(L"%.2f, %.2f, %.2f, %.2f\n", c.m128_f32[0], c.m128_f32[1], c.m128_f32[2], c.m128_f32[3]);
}

