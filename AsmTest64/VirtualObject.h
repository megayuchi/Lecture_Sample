#pragma once
#include "IVirtualObject.h"

class CVirtualObject : public IVirtualObject
{
    DWORD m_dwRefCount = 0;
    DWORD m_dwStatus = (DWORD)(-1);
public:
    void Func0();
    void Func1();
    void Func2();
    void Func3();

    CVirtualObject();
    ~CVirtualObject();
};

