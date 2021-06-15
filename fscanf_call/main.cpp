#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>

// Passing variable number of arguements with x86/x64 asm
// written by megayuchi. http://megayuchi.wordpress.com , twitter: @dgtman , 2007, 2015

#pragma comment(lib, "legacy_stdio_definitions.lib")

#ifdef _X64_PLATFORM
extern "C" int	fscanf_stub_x64(FILE * fp, char* szFormat, void** ppVArgList, DWORD qwVArgNum);
extern "C" int	testfunc(INT64 x, INT64 y, float c, float d);
#define fscanf_stub fscanf_stub_x64
#else
#include "fscanf_stub_x86.h"
#define fscanf_stub fscanf_stub_x86
#endif



int main()
{

	FILE*		fp = fopen("test.txt", "rt");
	if (!fp)
		goto lb_return;

	char	buf0[64];
	char	buf1[64];
	char	buf2[64];
	char	buf3[64];
	char	buf4[64];

	void*	pArgList[5] = { 0 };

	pArgList[0] = buf0;
	pArgList[1] = buf1;
	pArgList[2] = buf2;
	pArgList[3] = buf3;
	pArgList[4] = buf4;


	//	fscanf(fp,"%s",buf0);
	//	int	iResult = fscanf_stub_x64(fp,"%s",pArgList,1);
	//	int	iResult = fscanf_stub_x64(fp,"%s %s",pArgList,2);
	//	fscanf(fp,"%s %s %s %s %s",buf0,buf1,buf2,buf3,buf4);
	//	int	iResult = varg_stub_x64(fp,"%s %s %s",pArgList,3);
	//	int	iResult = varg_stub_x64(fp,"%s %s %s %s",pArgList,4);
	int	iResult = fscanf_stub(fp, "%s %s %s %s %s", pArgList, 5);


	for (DWORD i = 0; i < 5; i++)
	{
		printf_s("%s\n", pArgList[i]);
	}
	//	fscanf(fp,"%f %f %f",buf0,buf1,buf2,buf3,buf4);
	fclose(fp);
	_getch();

lb_return:
	return 0;
}