#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include <SDKDDKVer.h>


#ifndef APP_FULLSCREEN
#   define APP_FULLSCREEN_NEVER         0
#   define APP_FULLSCREEN_ALWAYS        1

#   define APP_FULLSCREEN               APP_FULLSCREEN_NEVER
#endif
#ifndef APP_RESIZABLE
#   define APP_RESIZABLE_NEVER          0
#   define APP_RESIZABLE_ALWAYS         1

#   define APP_RESIZABLE                APP_RESIZABLE_NEVER
#endif

// Modify the following two macros to utilize other versions of OpenGL
#define APP_OPENGL_MAJOR_VERSION        4
#define APP_OPENGL_MINOR_VERSION        6

#define APP_OPENGL_DEBUG_NONE           0
#define APP_OPENGL_DEBUG_MINIMAL        1
#define APP_OPENGL_DEBUG_ALL            2
#define APP_OPENGL_DEBUG_SEVERITY       APP_OPENGL_DEBUG_MINIMAL

#if (defined _WIN64)
#   pragma message("Message: The compilation target is 64-bit ARM or x64.")
#elif (defined _WIN32)
#   pragma message("Message: The compilation target is 32-bit ARM or x86.")
#endif
#if (_MSVC_LANG == 201402L)
#   pragma message("Message: The C++ language standard targeted by the compiler is: C++14!")
#elif (_MSVC_LANG == 201703L)
#   pragma message("Message: The C++ language standard targeted by the compiler is: C++17!")
#endif
