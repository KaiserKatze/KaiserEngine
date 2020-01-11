// stdafx.cpp : source file that includes just the standard includes
// $safeprojectname$.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

// @see: https://docs.microsoft.com/en-us/windows/win32/debug/retrieving-the-last-error-code
void ErrorExit(LPTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf = NULL;
    LPVOID lpDisplayBuf = NULL;
    DWORD dw = GetLastError();

    {
        std::stringstream ss;
        ss
            << "$ ErrorExit(lpszFunction=\"" << lpszFunction << "\") = " << dw
            << std::endl;
        OutputDebugStringA(ss.str().c_str());
    }

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
#ifdef _DEBUG
    {
        std::stringstream ss;
        ss << "OPENGL: " << name << " = " << p << std::endl;
        OutputDebugStringA(ss.str().c_str());
    }
#endif
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

void DetectGLError(const char* function)
{
    GLenum err = glGetError();

    std::wstringstream ss;
    ss << "[GL ERROR> IDX:"
        << function
        << ' ';
    if (err != GL_NO_ERROR)
    {
        err &= 0xf;
        if (err < sizeof(codelist) / sizeof(LPCSTR))
            ss << codelist[err];
        else
            ss << "GL_UNKNOWN_ERROR";
    }
    else
        ss << "GL_NO_ERROR";
    ss << ' '
        << "0x" << std::hex << err << std::dec
        << std::endl;

    OutputDebugString(ss.str().c_str());
}

void DetectGLError(const std::stringstream& ss)
{
    DetectGLError(ss.str().c_str());
}
