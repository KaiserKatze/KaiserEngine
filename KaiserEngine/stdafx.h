// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#ifndef UNICODE
#   define UNICODE
#endif
#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <string>
#include <atomic>
#include <thread>
#include <type_traits>

// TODO: reference additional headers your program requires here
#include <gl/GL.h>
#pragma comment(lib, "OpenGL32.Lib")
#include <gl/GLU.h>
#pragma comment(lib, "GlU32.Lib")

#include "include/glext.h"
#include "include/wglext.h"

void ErrorExit(LPTSTR lpszFunction);

// @see: https://stackoverflow.com/a/25231384/4927212
template<typename BaseType, typename T>
inline bool instanceof(const T*)
{
    return std::is_base_of<BaseType, T>::value;
}

void * GetAnyGLFuncAddress(const char * name);
void CleanDll();
void DetectGLError(int mark);
