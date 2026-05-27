#pragma once

#include <clocale>
#include <locale>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

inline void setupConsoleUtf8() {
#ifdef _WIN32
    const char* term = getenv("TERM_PROGRAM");
    const char* msyscon = getenv("MSYSCON");
    bool isMintty = (term && strstr(term, "mintty")) || (msyscon && strstr(msyscon, "mintty"));

    if (!isMintty) {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    }
#endif

    try {
        std::locale::global(std::locale("en_US.UTF-8"));
    } catch (...) {
    }
    std::cout.imbue(std::locale());
    std::cerr.imbue(std::locale());
    std::clog.imbue(std::locale());

    std::setlocale(LC_ALL, ".UTF-8");
}