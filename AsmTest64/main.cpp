// AsmTest64.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include "TestCpp.h"
#include <conio.h>
#include "math.inl"
#include "MathCPP.h"
#include "MathASM.h"
#include "Util.h"
#include "QueryPerfCounter.h"
#include "VirtualObject.h"

void MathTest(DWORD dwVectorNum);
void CmpReplaceTest();
void BasicTest();
void TestCallVirtualFunc();

unsigned int GetMaxValue_ASM_XOR(const unsigned int* puiValueList, unsigned int uiCount);
unsigned int GetMaxValue_ASM_CMOV(const unsigned int* puiValueList, unsigned int uiCount);
unsigned int GetMaxValue_SSE(const unsigned int* puiValueList, unsigned int uiCount);
unsigned int GetMaxValue_C(const unsigned int* puiValueList, unsigned int uiCount);

#pragma optimize("gpsy",off)
int main()
{
    HANDLE hThread = GetCurrentThread();
	DWORD_PTR	OldThreadAffinityMask = SetThreadAffinityMask(hThread, 0b1);

    srand((unsigned int)GetTickCount64());

    QCInit();

    BasicTest();

    CmpReplaceTest();

    MathTest(1024 * 1024);

    TestCallVirtualFunc();

    wprintf_s(L"Press any key\n");
    _getch();
}
void BasicTest()
{
#ifdef WIN64
    INT64 c;
    c = Add_C(1, 2);
    c = Add_ASM_RBP(1, 2);
    
    char str[12] = {};
    FillString_ASM(str, (INT64)_countof(str));
#endif
    int k = 0;
   
}
void MathTest(DWORD dwVectorNum)
{
    MATRIX4     mat;
    FillMatrix(&mat);

    VECTOR4 v4Src;
    FillVector4(&v4Src);
    
    VECTOR4 v4Result_C = {};
    VECTOR4 v4Result_C_SSE = {};
    VECTOR4 v4Result_ASM = {};

    TransformVector4_VPTR2_C(&v4Result_C, &v4Src, &mat, 1);
    TransformVector4_VPTR2_C_SSE(&v4Result_C_SSE, &v4Src, &mat, 1);
    TransformVector4_VPTR2_ASM(&v4Result_ASM, &v4Src, &mat, 1);
    int a = 0;

    
    // begin test
	VECTOR4*	pv4ListSrc = (VECTOR4*)_aligned_malloc(sizeof(VECTOR4) * dwVectorNum, sizeof(VECTOR4));
	VECTOR4*	pv4ListDest = (VECTOR4*)_aligned_malloc(sizeof(VECTOR4) * dwVectorNum, sizeof(VECTOR4));
	if (0 != (DWORD_PTR)pv4ListSrc % sizeof(VECTOR4))
		__debugbreak();
	if (0 != (DWORD_PTR)pv4ListDest % sizeof(VECTOR4))
		__debugbreak();

	memset(pv4ListSrc, 0, sizeof(VECTOR4) * dwVectorNum);
	memset(pv4ListDest, 0, sizeof(VECTOR4) * dwVectorNum);

	for (DWORD i = 0; i < dwVectorNum; i++)
	{
		FillVector4(pv4ListSrc + i);
	}

    //
    LARGE_INTEGER	PrvCounter = {};
	float fElapsedTick = 0;

	DWORD64	PrvClock = 0;
	DWORD64 ElapsedClock = 0;
	float fAvgClocks = 0.0f;

	VECTOR4* pv4First = nullptr;
	VECTOR4* pv4Last = nullptr;

	wprintf_s(L"TransformVector4_VPTR2\n");

	// C / NoSIMD
	TransformVector4_VPTR2_C(pv4ListDest, pv4ListSrc, &mat, dwVectorNum);

	PrvCounter = QCGetCounter();
	PrvClock = __rdtsc();

	TransformVector4_VPTR2_C(pv4ListDest, pv4ListSrc, &mat, dwVectorNum);

	ElapsedClock = __rdtsc() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	fAvgClocks = (float)ElapsedClock / (float)(dwVectorNum);
	pv4First = pv4ListDest + 0;
	pv4Last = pv4ListDest + dwVectorNum - 1;

	wprintf_s(L"\tNoSIMD Vector[%u] = (%.2f, %.2f, %.2f, %.2f), Vector[%u] = (%.2f, %.2f, %.2f, %.2f)\n",
			  0, pv4First->x, pv4First->y, pv4First->z, pv4First->w,
			  dwVectorNum - 1, pv4Last->x, pv4Last->y, pv4Last->z, pv4Last->w);

	wprintf_s(L"\t%.4f ms Elapsed. Avg Clocks per Operation : %.2f clocks.\n", fElapsedTick, fAvgClocks);
    wprintf_s(L"\n");

	// C / SSE
	memset(pv4ListDest, 0, sizeof(VECTOR4) * dwVectorNum);
	TransformVector4_VPTR2_C_SSE(pv4ListDest, pv4ListSrc, &mat, dwVectorNum);

	PrvCounter = QCGetCounter();
	PrvClock = __rdtsc();

	TransformVector4_VPTR2_C_SSE(pv4ListDest, pv4ListSrc, &mat, dwVectorNum);

	ElapsedClock = __rdtsc() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	fAvgClocks = (float)ElapsedClock / (float)(dwVectorNum);
	pv4First = pv4ListDest + 0;
	pv4Last = pv4ListDest + dwVectorNum - 1;

	wprintf_s(L"\tSIMD(C) Vector[%u] = (%.2f, %.2f, %.2f, %.2f), Vector[%u] = (%.2f, %.2f, %.2f, %.2f)\n",
			  0, pv4First->x, pv4First->y, pv4First->z, pv4First->w,
			  dwVectorNum - 1, pv4Last->x, pv4Last->y, pv4Last->z, pv4Last->w);

	wprintf_s(L"\t%.4f ms Elapsed. Avg Clocks per Operation : %.2f clocks.\n", fElapsedTick, fAvgClocks);
    wprintf_s(L"\n");

    // ASM / SSE
    memset(pv4ListDest, 0, sizeof(VECTOR4) * dwVectorNum);
    TransformVector4_VPTR2_ASM(pv4ListDest, pv4ListSrc, &mat, dwVectorNum);

	PrvCounter = QCGetCounter();
	PrvClock = __rdtsc();

	TransformVector4_VPTR2_ASM(pv4ListDest, pv4ListSrc, &mat, dwVectorNum);

	ElapsedClock = __rdtsc() - PrvClock;
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	fAvgClocks = (float)ElapsedClock / (float)(dwVectorNum);
	pv4First = pv4ListDest + 0;
	pv4Last = pv4ListDest + dwVectorNum - 1;

	wprintf_s(L"\tSIMD(ASM) Vector[%u] = (%.2f, %.2f, %.2f, %.2f), Vector[%u] = (%.2f, %.2f, %.2f, %.2f)\n",
			  0, pv4First->x, pv4First->y, pv4First->z, pv4First->w,
			  dwVectorNum - 1, pv4Last->x, pv4Last->y, pv4Last->z, pv4Last->w);

	wprintf_s(L"\t%.4f ms Elapsed. Avg Clocks per Operation : %.2f clocks.\n", fElapsedTick, fAvgClocks);
	wprintf_s(L"\n");


	if (pv4ListSrc)
	{
		_aligned_free(pv4ListSrc);
		pv4ListSrc = nullptr;
	}
	if (pv4ListDest)
	{
		_aligned_free(pv4ListDest);
		pv4ListDest = nullptr;
	}
}

void CmpReplaceTest()
{
    DWORD64 PrvClock, ElapsedClock;

    const DWORD MAX_VALUE_NUM = 1024 * 1024;
    unsigned int* puiValueList = new unsigned int[MAX_VALUE_NUM];
    for (DWORD i = 0; i < MAX_VALUE_NUM; i++)
    {
        puiValueList[i] = rand();
        //puiValueList[i] = i;
    }
    // warming up
    unsigned int iMaxValueC = GetMaxValue_C(puiValueList, MAX_VALUE_NUM);
    
    PrvClock = __rdtsc();
    iMaxValueC = GetMaxValue_C(puiValueList, MAX_VALUE_NUM);
    ElapsedClock = __rdtsc() - PrvClock;
    wprintf_s(L"GetMaxValue_C() Result: %u, Elpased Clock : %I64u\n", iMaxValueC, ElapsedClock);

#if defined(WIN64)
    // warming up
    unsigned int iMaxValueASM_XOR = GetMaxValue_ASM64_XOR(puiValueList, MAX_VALUE_NUM);
    
    PrvClock = __rdtsc();
    iMaxValueASM_XOR = GetMaxValue_ASM64_XOR(puiValueList, MAX_VALUE_NUM);
    ElapsedClock = __rdtsc() - PrvClock;
    wprintf_s(L"GetMaxValue_ASM_XOR64() Result: %u, Elpased Clock : %I64u\n", iMaxValueASM_XOR, ElapsedClock);

    // warming up
    unsigned int iMaxValueASM_CMOV = GetMaxValue_ASM64_CMOV(puiValueList, MAX_VALUE_NUM);
    
    PrvClock = __rdtsc();
    iMaxValueASM_CMOV = GetMaxValue_ASM64_CMOV(puiValueList, MAX_VALUE_NUM);
    ElapsedClock = __rdtsc() - PrvClock;
    wprintf_s(L"GetMaxValue_ASM64_CMOV() Result: %u, Elpased Clock : %I64u\n", iMaxValueASM_CMOV, ElapsedClock);
#else
    // warming up
    unsigned int iMaxValueASM_XOR = GetMaxValue_ASM_XOR(puiValueList, MAX_VALUE_NUM);
    
    PrvClock = __rdtsc();
    iMaxValueASM_XOR = GetMaxValue_ASM_XOR(puiValueList, MAX_VALUE_NUM);
    ElapsedClock = __rdtsc() - PrvClock;
    wprintf_s(L"GetMaxValue_ASM_XOR() Result: %u, Elpased Clock : %I64u\n", iMaxValueASM_XOR, ElapsedClock);
    
    // warming up
    unsigned int iMaxValueASM_CMOV = GetMaxValue_ASM_CMOV(puiValueList, MAX_VALUE_NUM);
    
    PrvClock = __rdtsc();
    iMaxValueASM_CMOV = GetMaxValue_ASM_CMOV(puiValueList, MAX_VALUE_NUM);
    ElapsedClock = __rdtsc() - PrvClock;
    wprintf_s(L"GetMaxValue_ASM_CMOV() Result: %u, Elpased Clock : %I64u\n", iMaxValueASM_CMOV, ElapsedClock);
#endif    

    


    if (puiValueList)
    {
        delete[] puiValueList;
        puiValueList = nullptr;
    }  
}

void TestCallVirtualFunc()
{
    CVirtualObject* pVirtualObject = new CVirtualObject;
    if (pVirtualObject)
    {

        IVirtualObject* pObj = pVirtualObject;

        pObj->Func0();
        pObj->Func1();
        pObj->Func2();
        pObj->Func3();

        delete pVirtualObject;
        pVirtualObject = nullptr;
    }
}
INT64 Add_C(INT64 a, INT64 b)
{
	INT64 c = a + b;
	return c;
}

#pragma optimize("gpsy",on)
unsigned int GetMaxValue_C(const unsigned int* puiValueList, unsigned int uiCount)
{
    unsigned int r = 0;
    for (unsigned int i = 0; i < uiCount; i++)
    {
        if (r < puiValueList[i])
        {
            r = puiValueList[i];
        }
    }
    return r;
}
#if !defined(WIN64)
__declspec(naked) unsigned int GetMaxValue_ASM_XOR(const unsigned int* puiValueList, unsigned int uiCount)
{
    __asm
    {
        push ebp
        mov ebp,esp

        push esi
        push edi
        push ebx

        xor eax,eax
        mov esi,dword ptr[puiValueList]
        mov ecx,dword ptr[uiCount]
    lb_cmp_and_swap:
        xor edx,edx
        mov ebx,dword ptr[esi]
        cmp eax,ebx
        sbb edx, 0; edx - (0 + CF)

        ; edx = and mask
        mov edi, eax
        xor edi, ebx; edi = eax ^ ebx
        and edi, edx

        xor eax,edi
        xor ebx,edi

        add esi,4
        dec ecx
        jnz lb_cmp_and_swap

        pop ebx    
        pop edi
        pop esi
 
        mov esp,ebp
        pop ebp
        ret
    }
}

__declspec(naked) unsigned int GetMaxValue_ASM_CMOV(const unsigned int* puiValueList, unsigned int uiCount)
{
    __asm
    {
        push ebp
        mov ebp, esp

        push esi

        xor eax,eax
        mov esi, dword ptr[puiValueList]
        mov ecx, dword ptr[uiCount]
    }
lb_cmp_and_swap:
    __asm
    {
        mov edx,dword ptr[esi]
        cmp edx,eax
        cmovae eax,edx

        add esi,4
        dec ecx
        jnz lb_cmp_and_swap

        pop esi
 
        mov esp,ebp
        pop ebp
        ret
    }
}
#endif