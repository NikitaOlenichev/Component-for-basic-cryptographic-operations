#pragma once

#include <clocale>

#ifdef _WIN32
#include <windows.h>
#endif

inline void setupConsoleUtf8() {
    std::setlocale(LC_ALL, "");

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}
