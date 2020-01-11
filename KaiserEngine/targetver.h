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
