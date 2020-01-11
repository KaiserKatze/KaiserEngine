#include "stdafx.h"
#include "Window.h"
#include "resource.h"
#include "loadgl.h"

// @see: https://github.com/LWJGL/lwjgl/blob/master/src/native/windows/opengles/context.c

constexpr size_t MAX_LOADSTRING = 127;

static int CreateFakeContext(const HWND& hWnd)
{
    //===========================================
    // Create context

    if (wglGetCurrentDC())
        return -1;

    HDC hDC = GetDC(hWnd);
    if (hDC == nullptr) return -1;
    wglMakeCurrent(hDC, nullptr);

    // @see: https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
    // Good pixel format to choose for the dummy context
    //  * 32-bit RGBA color buffer
    //  * 24-bit depth color buffer
    //  * 8-bit stencil
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        8,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int format = ChoosePixelFormat(hDC, &pfd);
    if (format == 0)
    {
        // could not find a pixel format that matches the description,
        // or the PDF was not filled out correctly
        ErrorExit(L"ChoosePixelFormat");
        return -1;
    }

    if (!SetPixelFormat(hDC, format, &pfd))
    {
        ErrorExit(L"CreateFakeContext -> SetPixelFormat");
        return -1;
    }

    HGLRC hRC = wglCreateContext(hDC);
    if (hRC == nullptr)
    {
        ErrorExit(L"wglCreateContext");
        return -1;
    }

    // essential for `LoadOpenglFunctions()`
    if (!wglMakeCurrent(hDC, hRC))
    {
        ErrorExit(L"wglMakeCurrent");
        return -1;
    }

    //===========================================
    // Get WGL Extensions

    LoadOpenglFunctions();

    //===========================================
    // Destroy context

    wglMakeCurrent(hDC, nullptr);
    wglDeleteContext(hRC);

    return 0;
}

static int CreateTrueContext(const HWND& hWnd)
{
    //===========================================
    // Create context

    if (wglGetCurrentDC())
        return -1;

    HDC hDC = GetDC(hWnd);
    if (hDC == nullptr) return -1;
    wglMakeCurrent(hDC, nullptr);

    AttributeListFactory pff;
    pff.put(WGL_DRAW_TO_WINDOW_ARB, GL_TRUE);
    pff.put(WGL_SUPPORT_OPENGL_ARB, GL_TRUE);
#if 0
    pff.put(WGL_TRANSPARENT_ARB, GL_TRUE);
    pff.put(WGL_DOUBLE_BUFFER_ARB, GL_FALSE); // Nvidia driver
    pff.put(WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB);
    pff.put(WGL_COLOR_BITS_ARB, 32);
    pff.put(WGL_ALPHA_BITS_ARB, 8);
    pff.put(WGL_DEPTH_BITS_ARB, 24);
    pff.put(WGL_STENCIL_BITS_ARB, 8);
    //pff.put(WGL_SAMPLE_BUFFERS_ARB, 1);
#endif
    const std::vector<int> attribList_pf = pff.make();
    //const FLOAT pfAttribFList[] = { 0 };
    int pixelFormat{ 0 };
    UINT numFormats{ 0 };
    int format = wglChoosePixelFormatARB(hDC, attribList_pf.data(), nullptr, 1, &pixelFormat, &numFormats);
    if (format == 0)
    {
        ErrorExit(L"wglChoosePixelFormatARB");
        return -1;
    }

    PIXELFORMATDESCRIPTOR pfd = { 0 };
    // @see: https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-describepixelformat
    if (DescribePixelFormat(hDC, pixelFormat, sizeof(pfd), &pfd) == 0)
    {
        ErrorExit(L"DescribePixelFormat");
        return -1;
    }

    // @see: https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-setpixelformat
    if (!SetPixelFormat(hDC, format, &pfd))
    {
        ErrorExit(L"CreateTrueContext -> SetPixelFormat");
        return -1;
    }

    // @see: https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_create_context.txt
    const int attribList_context[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, APP_OPENGL_MAJOR_VERSION, // opengl major version
        WGL_CONTEXT_MINOR_VERSION_ARB, APP_OPENGL_MINOR_VERSION, // opengl minor version
        WGL_CONTEXT_PROFILE_MASK_ARB, (WGL_CONTEXT_CORE_PROFILE_BIT_ARB), // opengl profile
        WGL_CONTEXT_FLAGS_ARB, (WGL_CONTEXT_DEBUG_BIT_ARB // debug context
                                | WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB),
        0, // End
    };
    HGLRC hRC = wglCreateContextAttribsARB(hDC, nullptr, attribList_context);
    if (hRC == nullptr)
    {
        ErrorExit(L"wglCreateContextAttribsARB");
        return -1;
    }

    if (!wglMakeCurrent(hDC, hRC))
    {
        ErrorExit(L"wglMakeCurrent");
        return -1;
    }

    return 0;
}


FakeWindow::
FakeWindow(const HINSTANCE& hInstance)
{
    wchar_t szWindowClass[MAX_LOADSTRING];
    wchar_t szTitle[MAX_LOADSTRING];
    LoadStringW(hInstance, IDC_KAISERENGINE_OPENLOADER, szWindowClass, MAX_LOADSTRING);
    LoadStringW(hInstance, IDS_APP_TITLE_LOADING, szTitle, MAX_LOADSTRING);

    std::exception fail_to_create("Fail to create FakeWindow instance!");

    if (!Create(hInstance, HWND_MESSAGE, szWindowClass, szTitle, 0, 0, 0, 0, 0, 0, SW_HIDE))
        throw fail_to_create;
    const HWND handle = getWindowHandle();
    if (!handle)
        throw std::exception("AssertionError: getWindowHandle() returns nullptr!");
    // disable user input for fake window
    EnableWindow(handle, false);
    if (CreateFakeContext(handle) != 0)
        throw fail_to_create;
    // destroy fake window after initializing opengl
    DestroyWindow(handle);
}

FakeWindow::
~FakeWindow()
{
}


MainWindow::
MainWindow(const HINSTANCE& hInstance)
{
    wchar_t szTitle[MAX_LOADSTRING];
    wchar_t szWindowClass[MAX_LOADSTRING];
    LoadStringW(hInstance, IDC_KAISERENGINE, szWindowClass, MAX_LOADSTRING);
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

    if (Create(hInstance, nullptr,
        szWindowClass, szTitle, 0, 0,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        SW_HIDE))
    {
        const HWND handle = getWindowHandle();
        EnableWindow(handle, true);
        CreateTrueContext(handle);
        
        // allow user input
        EnableWindow(handle, true);
        // setup event manager
        Setup(handle);
        // show window
        ShowWindow(true);
        // force the main window in front of all other windows
        SetForegroundWindow(handle);

        canvas = new Canvas();
        canvas->setParent(this);
        canvas->prepare();

        // install timer
        timerId = SetTimer(hWnd,
            0, // timer id
            20, // timeout value, in milliseconds;
                // this configuration setup fixs frame rate
                // 1000 MS = 1 FPS
                // 33 MS = 30.3 FPS
                // 20 MS = 50 FPS
                // 16 MS = 62.5 FPS
            nullptr); // make system post WM_TIMER message
        // returned timer id should be 1,
        // if no timer has been created yet
        if (timerId == 0)
        {
            throw std::exception("Fail to create a timer for MainWindow instance!");
        }
    }
    else
    {
        throw std::exception("Fail to create MainWindow instance!");
    }
}

MainWindow::
~MainWindow()
{
    if (canvas)
    {
        delete canvas;
        canvas = nullptr;
    }
    const HWND handle = getWindowHandle();
    if (handle)
    {
        if (timerId)
        {
            KillTimer(handle, timerId);
            timerId = 0;
        }
        DestroyWindow(handle);
        setWindowHandle(nullptr);
        PostQuitMessage(0);
    }
}

const Canvas*
MainWindow::
getCanvas() const
{
    return canvas;
}


AttributeListFactory::
AttributeListFactory()
{
}

AttributeListFactory::
~AttributeListFactory()
{
}

void
AttributeListFactory::
put(const int& key, const int& val)
{
    entries[key] = val;
}

void
AttributeListFactory::
clear()
{
    entries.clear();
}

size_t
AttributeListFactory::
size() const
{
    return entries.size() * 2 + 1;
}

const std::vector<int>
AttributeListFactory::
make()
{
    std::vector<int> attribVector;

    for (auto itr = entries.begin();
        itr != entries.end();
        itr++)
    {
        attribVector.push_back(itr->first);
        attribVector.push_back(itr->second);
    }

    attribVector.push_back(0); // push 0 as AttributeList ending

    return attribVector;
}
