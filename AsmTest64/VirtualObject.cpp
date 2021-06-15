#include "pch.h"
#include "VirtualObject.h"


#pragma optimize("gpsy",off)
CVirtualObject::CVirtualObject()
{
}
void CVirtualObject::Func0()
{
	// rcx = this ptr
	// [rsp+8] = rcx , this ptr
	// mov	QWORD PTR [rsp+8], rcx
	m_dwStatus = 0;
}
void CVirtualObject::Func1()
{
	// rcx = this ptr
	// [rsp+8] = rcx , this ptr
	// mov	QWORD PTR [rsp+8], rcx
	m_dwStatus = 1;
}
void CVirtualObject::Func2()
{
	// rcx = this ptr
	// [rsp+8] = rcx , this ptr
	// mov	QWORD PTR [rsp+8], rcx
	m_dwStatus = 2;
}
void CVirtualObject::Func3()
{
	// rcx = this ptr
	// [rsp+8] = rcx , this ptr
	// mov	QWORD PTR [rsp+8], rcx
	m_dwStatus = 3;
}
CVirtualObject::~CVirtualObject()
{
}

#pragma optimize("gpsy",on)