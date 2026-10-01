#pragma once
#ifdef Q_OS_WIN
#include <windows.h>
namespace cleanflow {
struct Win32Window final {
    HWND handle = nullptr;
    DWORD processId = 0;
};
}
#endif