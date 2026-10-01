#pragma once
#ifdef Q_OS_WIN
#include <windows.h>
#endif
namespace cleanflow {
#ifdef Q_OS_WIN
struct Win32Window { HWND handle=nullptr; unsigned long processId=0; };
#endif
} // namespace cleanflow
