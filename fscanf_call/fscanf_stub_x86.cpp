#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

#ifndef _X64_PLATFORM
extern "C"
{

int	fscanf_stub_x86(FILE* fp,char* szFormat,void** ppVArgList,DWORD dwVArgNum)
{
	int				iResult;
	__asm
	{
		mov			esi, dword ptr[ppVArgList]
		mov			ecx, dword ptr[dwVArgNum]
		mov			eax, ecx

		dec			eax
		shl			eax, 2
		add			esi, eax; ppVArgList[dwVAgrNum - 1]
	}
lb_loop:
	__asm
	{
		mov			eax,dword ptr[esi]
		push		eax
		sub			esi,4
		
		dec			ecx
		jnz			lb_loop

		mov			eax,dword ptr[szFormat]
		mov			edx,dword ptr[fp]
		push		eax
		push		edx

		call		dword ptr[fscanf]

		mov			[iResult],eax

		mov			eax,dword ptr[dwVArgNum]
		add			eax,2
		shl			eax,2
		
		add			esp,eax
	}
	return iResult;
}
}
#endif