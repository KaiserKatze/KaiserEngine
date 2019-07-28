// KaiserEngine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "KaiserEngine.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_KAISERENGINE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KAISERENGINE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KAISERENGINE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_KAISERENGINE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       0, 0, // initial position (x, y)
       1280, 720, // initial size (width, height)
       nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

int InitPixelFormat(HDC hdc)
{
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

    int format = ChoosePixelFormat(hdc, &pfd);
    if (format == 0)
    {
        // could not find a pixel format that matches the description,
        // or the PDF was not filled out correctly
        ErrorExit(L"ChoosePixelFormat");
        return -1;
    }

    if (!SetPixelFormat(hdc, format, &pfd))
    {
        ErrorExit(L"SetPixelFormat");
        return -1;
    }

    return 0;
}

#include "loadgl.h"

int RecreateContext(HDC hdc)
{
    const int attribList_pixel_format[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        0, // End
    };
    //const FLOAT pfAttribFList[] = { 0 };
    int pixelFormat;
    UINT numFormats;
    int format = wglChoosePixelFormatARB(hdc, attribList_pixel_format, (const FLOAT *) NULL, 1, &pixelFormat, &numFormats);
    if (format == 0)
    {
        ErrorExit(L"wglChoosePixelFormatARB");
        return -1;
    }

    if (SetPixelFormat(hdc, format, NULL))
    {
        ErrorExit(L"SetPixelFormat");
        return -1;
    }

    const int attribList_context[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4, // opengl major version
        WGL_CONTEXT_MINOR_VERSION_ARB, 6, // opengl minor version
        WGL_CONTEXT_PROFILE_MASK_ARB, (WGL_CONTEXT_CORE_PROFILE_BIT_ARB), // opengl profile
        WGL_CONTEXT_FLAGS_ARB, (WGL_CONTEXT_DEBUG_BIT_ARB), // debug context
        0, // End
    };
    HGLRC hglrc = wglCreateContextAttribsARB(hdc, (HGLRC) NULL, attribList_context);
    if (hglrc == NULL)
    {
        ErrorExit(L"wglCreateContextAttribsARB");
        return -1;
    }

    if (!wglMakeCurrent(hdc, hglrc))
    {
        ErrorExit(L"wglMakeCurrent");
        return -1;
    }

    return 0;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            HDC hDC = GetDC(hWnd);
            if (hDC == NULL) return -1;
            if (InitPixelFormat(hDC) < 0) return -1;
            HGLRC hRC = wglCreateContext(hDC);
            if (hRC == NULL)
            {
                ErrorExit(L"wglCreateContext");
                return -1;
            }
            if (!wglMakeCurrent(hDC, hRC))
            {
                ErrorExit(L"wglMakeCurrent");
                return -1;
            }

            //MessageBoxA(0, (char*)glGetString(GL_VERSION), "OPENGL VERSION", 0);
            //MessageBoxA(0, (char*)glGetString(GL_EXTENSIONS), "OPENGL EXTENSIONS", 0);
            //PROC proc = wglGetProcAddress("wglGetExtensionsStringARB");

            // Get WGL Extensions
            LoadOpenglFunctions();

            // Clean up dummy context
            if (!wglMakeCurrent(hDC, NULL))
            {
                ErrorExit(L"wglMakeCurrent");
                return -1;
            }
            if (!wglDeleteContext(hRC))
            {
                ErrorExit(L"wglDeleteContext");
                return -1;
            }

            // Recreate context
            if (RecreateContext(hDC) < 0) return -1;
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        {
            CleanDll();
            HDC hDC = wglGetCurrentDC();
            HGLRC hRC = wglGetCurrentContext();
            wglMakeCurrent(hDC, NULL);
            wglDeleteContext(hRC);

            PostQuitMessage(0);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
