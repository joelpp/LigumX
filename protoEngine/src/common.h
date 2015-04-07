#ifndef COMMON_H
#define COMMON_H

// Cross-Platform variables
#if defined(_WIN32) || defined(_WIN64)
#   define LX_WIN32
#elif defined(__unix__) || defined (__unix) || defined(unix)
#   define LX_UNIX
#else
#   error "Unknown OS. Only Windows & Linux supported for now."
#endif

// Platform dependent defines & functions
#ifdef LX_UNIX
#define sprintf_s sprintf
#endif // LX_UNIX

#endif // COMMON_H
