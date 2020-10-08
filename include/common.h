#pragma once
#include <Windows.h>
#include <string>

namespace MainWindowNames {
    std::wstring const WIN_CLASS_NAME = L"Hello";
    std::wstring const WIN_TITLE_NAME = L"Test DirectX";
    UINT const WIN_WIDTH = 1000;
    UINT const WIN_HEIGHT = 1000;
}

namespace common
{
    inline void throwIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw 1;
        }
    }
}
