#include "stdafx.h"
#include "AbstractWindow.h"

RECT
AbstractWindow::
GetDisplayRect()
{
    RECT rectDisplay;
    GetWindowRect(GetDesktopWindow(), &rectDisplay);
    return rectDisplay;
}

// used in `RegisterWindowClass()`
UINT
AbstractWindow::
MakeWindowClassStyle()
{
    return CS_HREDRAW // Redraws the entire window if a movement or size adjustment changes the width of the client area
        | CS_VREDRAW // Redraws the entire window if a movement or size adjustment changes the height of the client area
        | CS_OWNDC // enable GC, necessary for OpenGL
        | CS_DBLCLKS // decides whether or not to handle double-click mouse event
        ;
}

// used in `RegisterWindowClass()`
HBRUSH
AbstractWindow::
MakeBackgroundColor()
{
    return CreateSolidBrush(RGB(0, 0, 0)); // Produce black color
}

bool
AbstractWindow::
RegisterWindowClass(HINSTANCE hInstance, WNDPROC wndproc, LPCWSTR lpClass)
{
    if (hInstance == nullptr)
        throw std::exception("Invalid argument: 'hInstance' is nullptr!");
    if (wndproc == nullptr)
        throw std::exception("Invalid argument: 'wndproc' is nullptr!");
    if (lpClass == nullptr)
        throw std::exception("Invalid argument: 'lpClass' is nullptr!");

    // @see: https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
    WNDCLASSEX wcex = { 0 };

    wcex.cbSize = sizeof(WNDCLASSEX);
    // window style
    wcex.style = MakeWindowClassStyle();
    // window procedure callback handling events
    wcex.lpfnWndProc = wndproc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    // load icon
    wcex.hIcon = nullptr;
    // load cursor
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    // background color
    wcex.hbrBackground = MakeBackgroundColor();
    // menu
    wcex.lpszMenuName = nullptr;
    // window class
    wcex.lpszClassName = lpClass;
    // load small icon
    wcex.hIconSm = nullptr;

    // @see: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassexw
    ATOM res = RegisterClassEx(&wcex);
    if (res == 0)
    {
        ErrorExit(L"RegisterClassEx");
        return false;
    }
    return true;
}

// used in `InitWindowInstance()`
DWORD
AbstractWindow::
MakeWindowStyle()
{
    // window style
    // @see: https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles
    DWORD winStyle;
    winStyle = WS_OVERLAPPEDWINDOW;
    winStyle ^= WS_THICKFRAME; // WS_THICKFRAME grants that the window has a sizing border
    winStyle |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
    return winStyle;
}

// Create HWND, aka window handle
bool
AbstractWindow::
InitWindowInstance(HINSTANCE hInstance,
    HWND parent,
    LPCWSTR lpClass, LPCWSTR lpTitle,
    int x, int y,
    int width, int height,
    int nCmdShow)
{
    if (hInstance == nullptr)
        throw std::exception("Invalid argument: 'hInstance' is nullptr!");
    if (lpClass == nullptr)
        throw std::exception("Invalid argument: 'lpClass' is nullptr!");
    if (lpTitle == nullptr)
        throw std::exception("Invalid argument: 'lpTitle' is nullptr!");

    hWnd = CreateWindow(lpClass, lpTitle,
        MakeWindowStyle(),
        x, y, // initial position
        width, height, // initial size
        parent, // parent window
        nullptr, // menu
        hInstance, // instance handle
        this); // additional application data

#ifdef _DEBUG
    // Set window title
    {
        std::wstringstream wss;
        wss << lpTitle
            << " (hWnd=" << static_cast<void*>(hWnd)
            << ")";
        SetWindowTextW(hWnd, wss.str().c_str());
    }
#endif

    if (!hWnd)
    {
        ErrorExit(L"CreateWindow");
        return false;
    }

    // @see: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
    ::ShowWindow(hWnd, nCmdShow);

    isWindowActivated = true;

    return true;
}

bool
AbstractWindow::
Create(
    HINSTANCE hInstance,
    HWND parent,
    LPCWSTR lpClass,
    LPCWSTR lpTitle,
    DWORD dwStyle,
    DWORD dwExStyle,
    WNDPROC wndproc,
    int x,
    int y,
    int w,
    int h,
    int nCmdShow
)
{
#ifdef _DEBUG
    std::wstringstream ss;
    ss << "AbstractWindow::Create(" << std::endl
        << "\thInstance=" << static_cast<void*>(hInstance) << "," << std::endl
        << "\tparent=" << static_cast<void*>(parent) << "," << std::endl
        << "\tlpClass=" << lpClass << "," << std::endl
        << "\tlpTitle=" << lpTitle << "," << std::endl
        << "\tdwStyle=" << dwStyle << "," << std::endl
        << "\tdwExStyle=" << dwExStyle << "," << std::endl
        << "\tx=" << x << "," << std::endl
        << "\ty=" << y << "," << std::endl
        << "\tw=" << w << "," << std::endl
        << "\th=" << h << "," << std::endl
        << "\tnCmdShow=" << nCmdShow << ");";
#endif

    bool result;

    if (RegisterWindowClass(hInstance, wndproc, lpClass)
        && InitWindowInstance(hInstance, parent, lpClass, lpTitle, x, y, w, h, nCmdShow))
    {
        SetFocus(hWnd);
#ifdef _DEBUG
        ss << "\t... Success!" << std::endl;
#endif
        result = true;
    }
    else
    {
#ifdef _DEBUG
        ss << "\t... Failure!" << std::endl;
#endif
        result = false;
    }

    OutputDebugStringW(ss.str().c_str());
    return result;
}

AbstractWindow::
AbstractWindow() :
    hWnd(nullptr),
    hdc(nullptr),
    hglrc(nullptr),
    isFullscreen(false),
    isResizable(false),
    isWindowActivated(false),
    isWindowClosing(false)
{}

AbstractWindow::
~AbstractWindow() {}

HWND
AbstractWindow::
GetWindowHandle() const
{
    return hWnd;
}

void
AbstractWindow::
SetWindowHandle(HWND windowHandle)
{
    hWnd = windowHandle;
}

HDC
AbstractWindow::
GetDeviceContext() const
{
    return hdc;
}

void
AbstractWindow::
SetDeviceContext(HDC handle)
{
    hdc = handle;
}

HGLRC
AbstractWindow::
GetRenderContext() const
{
    return hglrc;
}

void
AbstractWindow::
SetRenderContext(HGLRC handle)
{
    hglrc = handle;
}

#if (defined _DEBUG) || ((APP_FULLSCREEN != APP_FULLSCREEN_ALWAYS) && (APP_FULLSCREEN != APP_FULLSCREEN_NEVER))
bool
AbstractWindow::
SetFullscreen(bool isFullscreen)
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
        wndStyle = WS_OVERLAPPEDWINDOW;
        if (!this->isResizable)
            wndStyle ^= WS_THICKFRAME;
    }
    wndStyle |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
    if (SetWindowLong(hWnd, GWL_STYLE, wndStyle) == 0)
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
            | SWP_DRAWFRAME // Draws a frame (defined in the window's class description) around the window
        ) == 0)
        {
            return false;
        }
    }

    return true;
}
#endif

#if (defined _DEBUG) || ((APP_RESIZABLE != APP_RESIZABLE_ALWAYS) && (APP_RESIZABLE != APP_RESIZABLE_NEVER))
bool
AbstractWindow::
SetResizable(bool isResizable)
{
    if (isFullscreen)
        return false;
    if (this->isResizable == isResizable)
        return true;
    this->isResizable = isResizable;

    LONG wndStyle;
    wndStyle = GetWindowLong(hWnd, GWL_STYLE);
    if (wndStyle == 0)
    {
        return false;
    }

    if (isResizable)
    {
        if (!isFullscreen)
            wndStyle |= WS_THICKFRAME;
    }
    else
    {
        wndStyle ^= WS_THICKFRAME;
    }

    if (SetWindowLong(hWnd, GWL_STYLE, wndStyle) == 0)
    {
        return false;
    }

    UINT uFlags;
    uFlags = SWP_NOZORDER // Retains the current Z order
        | SWP_NOREPOSITION // Does not change the owner window's position in the Z order
        | SWP_NOMOVE // Retains the current position (ignores X and Y parameters)
        | SWP_NOOWNERZORDER // Does not change the owner window's position in the Z order
        | SWP_NOSIZE // Retains the current size (ignores the cx and cy parameters)
        | SWP_NOCOPYBITS; // Discards the entire contents of the client area
    if (!isFullscreen)
    {
        uFlags |= SWP_DRAWFRAME; // Draws a frame (defined in the window's class description) around the window
    }
    if (SetWindowPos(
        hWnd,
        HWND_TOP,
        0, 0, 0, 0,
        uFlags) == 0)
    {
        return false;
    }

    return true;
}
#endif

void
AbstractWindow::
Show(bool show) const
{
    ::ShowWindow(GetWindowHandle(), show ? SW_SHOW : SW_HIDE);
}
