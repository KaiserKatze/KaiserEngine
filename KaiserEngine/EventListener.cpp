#include "stdafx.h"
#include "EventListener.h"
#include "UserInput.h"
#include "Window.h"

EventListener::
EventListener(const EventHandler& handler, const HWND& handle)
    : hdlr{ handler }
    , hWnd{ handle }
{
}

EventListener::
~EventListener()
{
}

const EventHandler&
EventListener::
getEventHandler() const
{
    return hdlr;
}

const HWND&
EventListener::
getWindowHandle() const
{
    return hWnd;
}

KeyboardEventListener::
KeyboardEventListener(const EventHandler& hdlr, const HWND& hWnd)
    : EventListener(hdlr, hWnd)
{
}

KeyboardEventListener::
~KeyboardEventListener()
{}

LRESULT
KeyboardEventListener::
Handle(UINT message, WPARAM wParam, LPARAM lParam) const
{
    switch (message)
    {
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
        return HandleKeyboardInput(getWindowHandle(), message, wParam, lParam);
    case WM_INPUTLANGCHANGE:
        {
            std::stringstream ss;
            ss << "WM_INPUTLANGCHANGE";
            ss << std::endl;
            OutputDebugStringA(ss.str().c_str());
        }
        return DefWindowProc(getWindowHandle(), message, wParam, lParam);
    default:
        return 0;
    }
}

MouseEventListener::
MouseEventListener(const EventHandler& hdlr, const HWND& hWnd)
    : EventListener(hdlr, hWnd)
{
}

MouseEventListener::
~MouseEventListener()
{}

LRESULT
MouseEventListener::
Handle(UINT message, WPARAM wParam, LPARAM lParam) const
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
    case WM_XBUTTONDBLCLK:
    case WM_MOUSEMOVE:
        return HandleMouseInput(getWindowHandle(), message, wParam, lParam);
    default:
        return 0;
    }
}

LRESULT
MouseEventListener::
OnClick(const MouseEvent) const
{
    return 0;
}

WindowEventListener::
WindowEventListener(const EventHandler& hdlr, const HWND& hWnd)
    : EventListener(hdlr, hWnd)
{
}

WindowEventListener::
~WindowEventListener()
{}

LRESULT
WindowEventListener::
Handle(UINT message, WPARAM wParam, LPARAM lParam) const
{
    switch (message)
    {
    case WM_CREATE:
        return OnCreate();
    case WM_CLOSE:
        return OnClose();
    case WM_DESTROY:
        return OnDestroy();
    case WM_SIZE:
        return OnResize(LOWORD(lParam), HIWORD(lParam));
        // @see: https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-activateapp
        // Sent when a window belonging to a different application than the active window is
        // about to be activated. The message is sent to the application whose window is being
        // activated and to the application whose window is being deactivated.
    case WM_ACTIVATEAPP:
        // Indicates whether the window is being activated or deactivated. This parameter is
        // TRUE if the window is being activated; it is FALSE if the window is being deactivated.
        //
        // this can be used to prevent CPU "hogging" when it is not necessary
        return OnActivate(static_cast<bool>(wParam));
    case WM_PAINT:
        return OnPaint();
    case WM_TIMER:
        return OnTimer();
    default:
        return 0;
    }
}

LRESULT
WindowEventListener::
OnCreate() const
{
    if (const MainWindow* window = getEventHandler().AsWindow<MainWindow>())
    {
        window->getCanvas()->prepare();
    }
    return 0;
}

LRESULT
WindowEventListener::
OnClose() const
{
    const HDC hDC = wglGetCurrentDC();
    const HGLRC hRC = wglGetCurrentContext();
    wglMakeCurrent(hDC, nullptr);
    wglDeleteContext(hRC);
    wglMakeCurrent(nullptr, nullptr);

    HWND hwnd = getWindowHandle();
    ReleaseDC(hwnd, hDC);
    ::ShowWindow(hwnd, SW_HIDE);

    return 0;
}

LRESULT
WindowEventListener::
OnDestroy() const
{
    // clean up opengl functions
    CleanDll();

    PostQuitMessage(0);
    return 0;
}

LRESULT
WindowEventListener::
OnResize(const int& newWidth, const int& newHeight) const
{
    if (const MainWindow* window = getEventHandler().AsWindow<MainWindow>())
    {
        window->getCanvas()->setup(newWidth, newHeight);
    }
    return 0;
}

LRESULT
WindowEventListener::
OnActivate(bool state) const
{
    return 0;
}

LRESULT
WindowEventListener::
OnPaint() const
{
    return 0;
}

LRESULT
WindowEventListener::
OnTimer() const
{
    //OutputDebugStringA("On drawing OpenGL ...\r\n");
    if (const MainWindow* window = getEventHandler().AsWindow<MainWindow>())
    {
        window->getCanvas()->render();
    }
    return 0;
}
