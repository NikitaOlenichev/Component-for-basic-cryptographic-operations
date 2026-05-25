#ifndef CRYPTOCOMPONENT_EXPORT_H
#define CRYPTOCOMPONENT_EXPORT_H

#ifdef _WIN32
    #ifdef CRYPTOCOMPONENT_BUILD_DLL
        #define CRYPTOCOMPONENT_API __declspec(dllexport)
    #else
        #define CRYPTOCOMPONENT_API __declspec(dllimport)
    #endif
#else
    #define CRYPTOCOMPONENT_API __attribute__((visibility("default")))
#endif

#endif