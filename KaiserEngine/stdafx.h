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
#include <windowsx.h>

// C RunTime Header Files
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <tchar.h>
#include <strsafe.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <string>
#include <atomic>
#include <thread>
#include <vector>
#include <map>
#include <tuple>
#include <array>
#include <type_traits>
#include <memory>
#include <cmath>
#include <initializer_list>
#include <algorithm>
#include <iterator>
#include <optional>

// TODO: reference additional headers your program requires here
#include <gl/GL.h>
#pragma comment(lib, "OpenGL32.Lib")
#include <gl/GLU.h>
#pragma comment(lib, "GlU32.Lib")

#include "include/glext.h"
#include "include/wglext.h"
using GLstring = const GLchar*;

void ErrorExit(LPTSTR lpszFunction);

void * GetAnyGLFuncAddress(const char * name);
void CleanDll();
void DetectGLError(const char*);
void DetectGLError(const std::stringstream&);
void SuppressGLError();
