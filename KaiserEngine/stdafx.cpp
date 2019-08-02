// stdafx.cpp : source file that includes just the standard includes
// $safeprojectname$.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

#include <strsafe.h>

// @see: https://docs.microsoft.com/en-us/windows/win32/debug/retrieving-the-last-error-code
void ErrorExit(LPTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf = NULL;
    LPVOID lpDisplayBuf = NULL;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error 0x%X: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw);
}

static HMODULE module = (HMODULE)NULL;

void * GetAnyGLFuncAddress(const char * name)
{
    void * p = (void *)wglGetProcAddress(name);
    if (p == (void *)0
        || (p == (void *)0x1)
        || (p == (void *)0x2)
        || (p == (void *)0x3)
        || (p == (void *)-1))
    {
        if (!module)
            module = LoadLibraryA("opengl32.dll");
        p = (void *)GetProcAddress(module, name);
    }

    return p;
}

void CleanDll()
{
    if (module)
    {
        FreeLibrary(module);
        module = (HMODULE)NULL;
    }
}

static LPCSTR const codelist[] = {
    "GL_NO_ERROR",
    "GL_INVALID_ENUM",
    "GL_INVALID_VALUE",
    "GL_INVALID_OPERATION",
    "GL_STACK_OVERFLOW",
    "GL_STACK_UNDERFLOW",
    "GL_OUT_OF_MEMORY"
};

void DetectGLError(int mark)
{
    GLenum err = glGetError();

    std::wstringstream ss;
    ss << "[GL ERROR> IDX:";
    ss << mark;
    ss << ' ';
    if (err != GL_NO_ERROR)
        ss << codelist[err & 0x1];
    else
        ss << "GL_NO_ERROR";
    ss << ' ';
    ss << err;
    ss << std::endl;

    OutputDebugString(ss.str().c_str());
    if (err != GL_NO_ERROR)
        MessageBox(nullptr, ss.str().c_str(), TEXT("OpenGL Error"), MB_OK);
}
