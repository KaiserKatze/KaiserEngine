#pragma once

#include "stdafx.h"
#include "resource.h"

RECT GetDisplayRect();

template <typename WindowType>
class BaseWindow
{
private:
    std::atomic_bool isFullscreen;
    std::atomic_bool isResizable;

    ATOM RegisterWindowClass(HINSTANCE hInstance, WNDPROC wndproc, LPCWSTR lpClass)
    {
        if (hInstance == nullptr)
            return 0;
        if (wndproc == nullptr)
            return 0;
        if (lpClass == nullptr)
            return 0;

        WNDCLASSEX wcex = { 0 };

        wcex.cbSize = sizeof(WNDCLASSEX);

        UINT wndStyle;
        wndStyle = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
#if (APP_USERINPUT)
#   if (APP_USERINPUT_DBLCLKS)
        wndStyle |= CS_DBLCLKS;
#   endif
#endif
        wcex.style = wndStyle;
        wcex.lpfnWndProc = wndproc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        // load icon
        wcex.hIcon = nullptr;
        // load cursor
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        // background color
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        // menu
        wcex.lpszMenuName = nullptr;
        // window class
        wcex.lpszClassName = lpClass;
        // load small icon
        wcex.hIconSm = nullptr;

        return RegisterClassEx(&wcex);
    }

    BOOL InitWindowInstance(HINSTANCE hInstance,
        LPCWSTR lpClass, LPCWSTR lpTitle,
        int x, int y,
        int width, int height)
    {
        if (hInstance == nullptr)
            return false;
        if (lpClass == nullptr)
            return false;
        if (lpTitle == nullptr)
            return false;

        // window style
        // @see: https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles
        DWORD winStyle;
        winStyle = WS_OVERLAPPEDWINDOW;
        winStyle ^= WS_THICKFRAME;
        winStyle |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

        hWnd = CreateWindow(lpClass, lpTitle,
            winStyle,
            x, y, // initial position
            width, height, // initial size
            nullptr, // parent window
            nullptr, // menu
            hInstance, // instance handle
            this); // additional application data

        if (!hWnd)
        {
            return false;
        }

        int nCmdShow;
        nCmdShow = SW_SHOW;
        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        isWindowActivated = true;

        return true;
    }
protected:
    HWND hWnd;
    std::atomic_bool isWindowActivated;
    std::atomic_bool isWindowClosing;

    virtual LRESULT CALLBACK HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        WindowType *pThis = NULL;

        if (message == WM_NCCREATE)
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (WindowType*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->hWnd = hwnd;
        }
        else
        {
            pThis = (WindowType*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        if (pThis)
        {
            return pThis->HandleMessage(message, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
    }

    void UpdateWindowTitle()
    {
        // change window title name
        GLint iMajorVersion, iMinorVersion;
        glGetIntegerv(GL_MAJOR_VERSION, &iMajorVersion);
        glGetIntegerv(GL_MINOR_VERSION, &iMinorVersion);

        std::wstringstream sNewTitle;
        sNewTitle << szTitle;
        sNewTitle << " (OpenGL version: ";
        sNewTitle << iMajorVersion << '.' << iMinorVersion;
        sNewTitle << ")";
        SetWindowText(hWnd, sNewTitle.str().c_str());
    }
public:
    BaseWindow() :
        hWnd(nullptr),
        isFullscreen(false),
        isResizable(false),
        isWindowActivated(false),
        isWindowClosing(false)
    {}

    HWND getWindowHandle()
    {
        return hWnd;
    }

    BOOL Create(
        HINSTANCE hInstance,
        LPCWSTR lpClass,
        LPCWSTR lpTitle,
        DWORD dwStyle = 0,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int w = CW_USEDEFAULT,
        int h = CW_USEDEFAULT
    )
    {
        return RegisterWindowClass(hInstance, WindowType::WindowProc, lpClass)
            && InitWindowInstance(hInstance, lpClass, lpTitle, x, y, w, h);
    }

#if (defined _DEBUG) || ((APP_FULLSCREEN != APP_FULLSCREEN_ALWAYS) && (APP_FULLSCREEN != APP_FULLSCREEN_NEVER))
    BOOL SetFullscreen(BOOL isFullscreen)
    {
        if (this->isFullscreen == isFullscreen)
            return true;
        this->isFullscreen = isFullscreen;

        // change window style
        LONG wndStyle;
        //wndStyle = GetWindowLong(hWnd, GWL_STYLE);
        if (isFullscreen)
        {
            wndStyle = WS_POPUP;
        }
        else
        {
            winStyle = WS_OVERLAPPEDWINDOW;
            if (!this->isResizable)
                winStyle ^= WS_THICKFRAME;
        }
        winStyle |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
        if (SetWindowLong(hWnd, GWL_STYLE, winStyle) == 0)
        {
            return false;
        }

        if (isFullscreen)
        {
            const RECT rectDisplay = GetDisplayRect();

            const int x = 0;
            const int y = 0;
            const int width = rectDisplay.right;
            const int height = rectDisplay.bottom;

            if (SetWindowPos(
                hWnd,
                HWND_TOP,
                x, y,
                width, height,
                SWP_NOZORDER // Retains the current Z order
                | SWP_NOREPOSITION // Does not change the owner window's position in the Z order
                | SWP_NOCOPYBITS // Discards the entire contents of the client area
                | SWP_FRAMECHANGED // Applies new frame styles set using the SetWindowLong function
            ) == 0)
            {
                return false;
            }
        }
        else
        {
            if (SetWindowPos(
                hWnd,
                HWND_TOP,
                0, 0, 0, 0,
                SWP_NOZORDER // Retains the current Z order
                | SWP_NOREPOSITION // Does not change the owner window's position in the Z order
                | SWP_NOCOPYBITS // Discards the entire contents of the client area
                | SWP_FRAMECHANGED // Applies new frame styles set using the SetWindowLong function
                | SWP_DRAWFRAME // Draws a frame (defined in the window's class description) around the window
            ) == 0)
            {
                return false;
            }
        }

        return true;
    }
#endif
};
