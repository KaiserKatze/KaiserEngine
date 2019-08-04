#include "stdafx.h"
#include "UserInput.h"

#if APP_USERINPUT
#include <windowsx.h>

// @see: https://docs.microsoft.com/en-us/windows/win32/learnwin32/mouse-clicks
LRESULT HandleMouseInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    const int x = GET_X_LPARAM(lParam);
    const int y = GET_Y_LPARAM(lParam);
    const BOOL isKeyDown_ctrl = wParam & MK_CONTROL; // check if the CTRL key is down
    const BOOL isKeyDown_shift = wParam & MK_SHIFT; // check if the SHIFT key is down
    // If you need to find the state of other keys besides CTRL and SHIFT, use the GetKeyState function
    std::stringstream ss;
    ss << "Mouse ";
    switch (message)
    {
    case WM_LBUTTONDOWN:
        ss << "LB down";
        break;
    case WM_LBUTTONUP:
        ss << "LB up";
        break;
    case WM_MBUTTONDOWN:
        ss << "MB down";
        break;
    case WM_MBUTTONUP:
        ss << "MB up";
        break;
    case WM_RBUTTONDOWN:
        ss << "RB down";
        break;
    case WM_RBUTTONUP:
        ss << "RB up";
        break;
    case WM_XBUTTONDOWN:
        ss << "XB down";
        break;
    case WM_XBUTTONUP:
        ss << "XB up";
        break;
    case WM_LBUTTONDBLCLK:
        ss << "LB double click";
        break;
    case WM_MBUTTONDBLCLK:
        ss << "MB double click";
        break;
    case WM_RBUTTONDBLCLK:
        ss << "RB double click";
        break;
    case WM_XBUTTONDBLCLK:
        ss << "XB double click";
        break;
    case WM_MOUSEMOVE:
        ss << "move";
        break;
    }
    ss << " @ (" << x << ", " << y << ")";
    ss << std::endl;
    OutputDebugStringA(ss.str().c_str());
    return 0;
}

LRESULT HandleKeyboardInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    const int keyCode = (wParam);
    // previous key state
    const int oldKeyCode = ((lParam >> 30) & 1);
    switch (message)
    {
    case WM_KEYDOWN:
    case WM_KEYUP:
        break;
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
#endif
