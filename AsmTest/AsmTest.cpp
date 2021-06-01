// AsmTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <stdint.h>
#include <conio.h>
#include <memory>
#include <stdlib.h>

class CTest
{
	int m_iCount = 0;
public:
	void Set(int value);
	int Add(int a, int b, int c, int d, int e, int f);
};
void CTest::Set(int value)
{
	m_iCount = value;
}
int CTest::Add(int a, int b, int c, int d, int e, int f)
{
	// ecx = this
	int sum = m_iCount + a + b + c + d + e + f;
	return sum;
}
enum CMP_RESULT
{
	A_EQUAL_B,
	A_GREATOR_B,
	A_LESS_B,
	A_ABOVE_B,
	A_BELOW_B,
	CMP_RESULT_COUNT
};

void CopyTest();

int Add_C(int a, int b);
int __cdecl Add_ASM_CDECL(int a, int b);
int __cdecl Add_NakedCall_CDECL(int a, int b);
int __cdecl Add_NakedCall_CDECL_LOCAL_VAR(int a, int b);
int __stdcall Add_NakedCall_STDCALL(int a, int b);

int Mul_C(int a, int b);
int __cdecl Mul_ASM(int a, int b);
int __cdecl Div_ASM(int a, int b);

CMP_RESULT CmpTestSigned(int a, int b);
CMP_RESULT CmpTestUnsigned(int a, int b);
CMP_RESULT __cdecl CmpTestUnsigned_CDECL(int a, int b);
CMP_RESULT __stdcall CmpTestUnsigned_STDCALL(int a, int b);

void TestCallMemcpy();
void TestCallIndirect();

void TestCmpTest();

int main()
{
	CTest _test;
	_test.Set(10);
	int sum = _test.Add(1, 2, 3, 4, 5, 6);

	CopyTest();

	int d = Mul_ASM(3, 4);
	d = Div_ASM(13, 4);

	int a = -1;
	int b = 0;
	int c = 0;
	
	// C함수
	c = Add_C(a, b);

	// ASM-cdecl
	c = Add_ASM_CDECL(a, b);

	// ASM-stdcall
	c = Add_NakedCall_STDCALL(a, b);

	// 로컬 변수 스택프레임
	c = Add_NakedCall_CDECL_LOCAL_VAR(a, b);

	// cmp 테스트
	TestCmpTest();

	// 직접 call테스트
	TestCallMemcpy();

	// 간접 call테스트
	TestCallIndirect();

	return 0;
}
void TestCallIndirect()
{	
	const WCHAR* wchCaption = L"Caption";
	const WCHAR* wchText = L"Text";
	
//	MessageBox(nullptr, wchText, wchCaption, MB_OK);

	__asm
	{
		mov eax, dword ptr[wchCaption]
		mov edx, dword ptr[wchText]
		push MB_OK
		push eax
		push edx
		push 0
		call dword ptr[MessageBox]
	}
	int a = 0;
}

void TestCallMemcpy()
{
	WCHAR	src[32] = L"TestCallMemcpy()";
	WCHAR	dest[32] = {};

	//memcpy(dest, src, 64);

	__asm
	{
		; src ptr
		lea eax, dword ptr[src]

		; dest ptr
		lea edx, dword ptr[dest]

		; size (sizeof(WCHAR) * 32 = 64)
		push 64;

		; src
		push eax
		
		; dest
		push edx
		
		call memcpy
		add esp,12
	}
	wprintf_s(L"src = %s\n", src);
	wprintf_s(L"dest = %s\n", dest);

	wprintf_s(L"Press any key.\n");
	_getch();
}

void TestCmpTest()
{
	
	const WCHAR* wchInequality[] = { L"==", L">", L"<", L">", L"<" };
	CMP_RESULT r = A_EQUAL_B;
	
	int a = -1;
	int b = 0;
	int c = 0;

	wprintf_s(L"Signed Test\n");
	r = CmpTestSigned(a, b);
	wprintf_s(L"CMP A, B -> %d %s %d\n", a, wchInequality[r], b);
	
	wprintf_s(L"\n");
	
	wprintf_s(L"Unsigned Test\n");
	r = CmpTestUnsigned(a, b);
	wprintf_s(L"CMP A, B -> %u %s %u\n", a, wchInequality[r], b);

	wprintf_s(L"Unsigned Test-Naked call-CDECL\n");
	r = CmpTestUnsigned_CDECL(a, b);
	wprintf_s(L"CMP A, B -> %u %s %u\n", a, wchInequality[r], b);

	wprintf_s(L"Unsigned Test-Naked call-STDCALL\n");
	r = CmpTestUnsigned_STDCALL(a, b);
	wprintf_s(L"CMP A, B -> %u %s %u\n", a, wchInequality[r], b);


	
	wprintf_s(L"Press any key.\n");
	_getch();
}
CMP_RESULT CmpTestSigned(int a, int b)
{	
	CMP_RESULT r = A_EQUAL_B;
	// signed - (a > b) -> jg (ZF = 0 and SF = OF)
	// signed - (a < b) -> jl (SF <> OF)
	// signed - (a == b) -> je (ZF = 1) 
	__asm
	{
		mov eax, dword ptr[a]
		mov edx, dword ptr[b];
		cmp eax, edx
		je lb_a_equal_b
		jg lb_a_greator_b
		jl lb_a_less_b
		int 3

	lb_a_equal_b:
		mov dword ptr[r], A_EQUAL_B
		jmp lb_exit

	lb_a_greator_b :
		mov dword ptr[r], A_GREATOR_B
		jmp lb_exit

	lb_a_less_b :
		mov dword ptr[r], A_LESS_B
		jmp lb_exit

	lb_exit :
		nop
	}
	return r;
	
}
CMP_RESULT CmpTestUnsigned(int a, int b)
{
	// unsigned ja (CF = 0 and ZF = 0)
	// unsigned jb (CF = 1)
	// signed - (a == b) -> je (ZF = 1) 
	CMP_RESULT r = A_EQUAL_B;

	__asm
	{
		mov eax, dword ptr[a]
		mov edx, dword ptr[b];
		cmp eax, edx
		je lb_a_equal_b
		ja lb_a_above_b
		jb lb_a_below_b
		int 3

	lb_a_equal_b:
		mov dword ptr[r], A_EQUAL_B
		jmp lb_exit

	lb_a_above_b :
		mov dword ptr[r], A_ABOVE_B
		jmp lb_exit

	lb_a_below_b :
		mov dword ptr[r], A_BELOW_B
		jmp lb_exit

	lb_exit :
		nop

	}
	return r;
}
CMP_RESULT __declspec(naked) __cdecl CmpTestUnsigned_CDECL(int a, int b)
{
	__asm
	{
		push ebp
		mov ebp,esp

		mov eax, dword ptr[a]
		mov edx, dword ptr[b];
		cmp eax, edx
		je lb_a_equal_b
		ja lb_a_above_b
		jb lb_a_below_b
		int 3

	lb_a_equal_b:
		mov eax, A_EQUAL_B
		jmp lb_exit

	lb_a_above_b :
		mov eax, A_ABOVE_B
		jmp lb_exit

	lb_a_below_b :
		mov eax, A_BELOW_B
		jmp lb_exit

	lb_exit :
		mov esp,ebp
		pop ebp
		ret
	}
}

CMP_RESULT __declspec(naked) __stdcall CmpTestUnsigned_STDCALL(int a, int b)
{
	__asm
	{
		push ebp
		mov ebp,esp

		mov eax, dword ptr[a]
		mov edx, dword ptr[b];
		cmp eax, edx
		je lb_a_equal_b
		ja lb_a_above_b
		jb lb_a_below_b
		int 3

	lb_a_equal_b:
		mov eax, A_EQUAL_B
		jmp lb_exit

	lb_a_above_b :
		mov eax, A_ABOVE_B
		jmp lb_exit

	lb_a_below_b :
		mov eax, A_BELOW_B
		jmp lb_exit

	lb_exit :
		mov esp,ebp
		pop ebp
		ret 8
	}
}
int Add_C(int a, int b)
{
	int c = a + b;
	return c;
}
int __cdecl Add_ASM_CDECL(int a, int b)
{
#define ARRAY_COUNT 4
	int temp[ARRAY_COUNT];
	__asm
	{
		mov ecx, ARRAY_COUNT
		lea edi,dword ptr[temp]
		xor eax,eax
		rep stosd
		mov eax, dword ptr[a];
		mov edx, dword ptr[b];
		add eax,edx
		mov dword ptr[temp],eax
	}
	return temp[0];
}
int __declspec(naked) __cdecl Add_NakedCall_CDECL(int a, int b)
{
	__asm
	{
		push ebp
		mov ebp, esp

		mov eax,dword ptr[a]
		mov edx,dword ptr[b]

		add eax,edx

		mov esp,ebp
		pop ebp

		ret
	}
}
int __declspec(naked) __stdcall Add_NakedCall_STDCALL(int a, int b)
{
	__asm
	{
		push ebp
		mov ebp, esp

		mov eax,dword ptr[a]
		mov edx,dword ptr[b]

		add eax,edx

		mov esp,ebp
		pop ebp

		ret 8
	}
}
int __declspec(naked) __cdecl Add_NakedCall_CDECL_LOCAL_VAR(int a, int b)
{
	// ebx, esi, edi must be preserved.
#define ARRAY_COUNT 4
	int temp[ARRAY_COUNT];
 	
	__asm
	{
		push ebp
		mov ebp, esp

		; local area
		sub esp, __LOCAL_SIZE

		; bakcup registers
		push edi

		; clear local variable
		; &temp[0] -> (ebp-20) , &temp[1] -> (ebp-16), &temp[2] -> (ebp-12), &temp[3] -> (ebp-8)
		mov ecx, ARRAY_COUNT
		xor eax, eax

		lea edi, dword ptr[temp]	// 
		rep stosd

		; a + b
		mov eax,dword ptr[a]
		mov edx,dword ptr[b]
		add eax,edx

		pop edi

		mov esp,ebp
		pop ebp
		ret
	}
}


int Mul_C(int a, int b)
{
	int c = a * b;
	return c;
}
int __declspec(naked) __cdecl Mul_ASM(int a, int b)
{
	__asm
	{
		push ebp
		mov ebp,esp

		mov eax,dword ptr[a]
		mov ecx,dword ptr[b]

		imul ecx
		
		mov esp,ebp
		pop ebp

		ret
	}
}
int __declspec(naked) __cdecl Div_ASM(int a, int b)
{
	__asm
	{
		push ebp
		mov ebp,esp

		mov eax,dword ptr[a]
		mov ecx,dword ptr[b]
		idiv ecx
		
		mov esp,ebp
		pop ebp

		ret
	}
}

void CopyTest()
{
	__declspec(align(16)) char szSrc[64] = "ABCDEFGHABCDEFGHABCDEFGHABCDEFGHABCDEFGHABCDEFGHABCDEFGHABCDEF.";
	__declspec(align(16)) char szDest[64] = {};
	__asm
	{
		lea esi, dword ptr[szSrc]
		lea edi, dword ptr[szDest]

		; copy 1byte, szDest[0] = szSrc[0]
		mov al, byte ptr[esi]
		mov byte ptr[edi], al

		; copy 2bytes, szDest[0 - 1] = szSrc[0 - 1]
		mov ax, word ptr[esi]
		mov word ptr[edi], ax

		; copy 4bytes, szDest[0 - 3] = szSrc[0 - 3]
		mov eax, dword ptr[esi]
		mov dword ptr[edi], eax

		; copy 4bytes * 16 , szDest[0 - 63] = szSrc[0 - 63]
		mov ecx, 64
		shr ecx,2
		rep movsd
		nop
	}
}