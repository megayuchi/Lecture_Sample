#pragma once

#ifndef _X64_PLATFORM
	extern "C" int	fscanf_stub_x86(FILE* fp,char* szFormat,void** ppVArgList,DWORD dwVArgNum);
#endif
