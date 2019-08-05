#include "stdafx.h"
#include "UserInput.h"

#if APP_USERINPUT
#include <windowsx.h>

// @see: https://docs.microsoft.com/en-us/windows/win32/learnwin32/mouse-clicks
LRESULT HandleMouseInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    const int x = GET_X_LPARAM(lParam);
    const int y = GET_Y_LPARAM(lParam);
    const bool isKeyDown_ctrl = wParam & MK_CONTROL; // check if the CTRL key is down
    const bool isKeyDown_shift = wParam & MK_SHIFT; // check if the SHIFT key is down
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
    // lParam: bit 30, the "previous key state" flag, which is set to 1 for repeated key-down messages
    const int oldKeyCode = ((lParam >> 30) & 1);
    switch (message)
    {
    case WM_KEYDOWN:
    case WM_KEYUP:
        {
            // Virtual-key code
            // @see: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
            int vk = (int)(0xff & wParam);
            // Repeat code
            // The number of times the keystroke is autorepeated
            // as a result of the user holding down the key.
            // The repeat count is always 1 for a WM_KEYUP message
            int rc = (int)(0xffff & lParam);
            // Scan code
            // The value depends on the OEM
            int sc = (int)(0x7f & (lParam >> 16));
            // Extended key
            // Indicates whether the key is an extended key, such as the right-hand ALT
            // and CTRL keys that appear on an enhanced 101- or 102-key keyboard
            // The value is 1 if it is an extended key; otherwise, it is 0
            int ek = (int)(0x1 & (lParam >> 24));
            // Context code
            // The value is always 0 for a WM_KEYUP message
            // The value is always 0 for a WM_KEYDOWN message
            int cc = (int)(0x1 & (lParam >> 29));
            // Previous key state
            // The value is always 1 for a WM_KEYUP message
            // The value is 1 if the key is down before the message is sent, or it is zero if the key is up
            int ks = (int)(0x1 & (lParam >> 30));
            // Transition state
            // The value is always 1 for a WM_KEYUP message
            // The value is always 0 for a WM_KEYDOWN message
            int ts = (int)(0x1 & (lParam >> 31));
        }
        break;
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
#endif
