#ifndef COMMON_H
#define COMMON_H

// Cross-Platform variables
#if defined(_WIN32) || defined(_WIN64)
#   define LX_WIN32
#elif defined(__unix__) || defined (__unix) || defined(unix)
#   define LX_UNIX
#elif defined(__APPLE__)
	#include "TargetConditionals.h"
    #ifdef TARGET_OS_IPHONE
         // iOS
    #elif TARGET_IPHONE_SIMULATOR
        // iOS Simulator
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
    #else
        // Unsupported platform
    #endif
#else
#   error "Unknown OS. Only Windows & Linux supported for now."
#endif

// Platform dependent defines & functions
#ifdef LX_UNIX
#define sprintf_s sprintf
#endif // LX_UNIX

#endif // COMMON_H

#ifdef __APPLE__
    #include "TargetConditionals.h"
    #ifdef TARGET_OS_IPHONE
         // iOS
    #elif TARGET_IPHONE_SIMULATOR
        // iOS Simulator
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
    #else
        // Unsupported platform
    #endif
#endif