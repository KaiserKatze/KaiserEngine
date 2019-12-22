#include "stdafx.h"
#include "Event.h"

static MouseButton TranslateButton(UINT message);
static POINT TranslatePoint(LPARAM lParam);

MouseEvent::
MouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
    : button{ TranslateButton(message) }
    , combination{ wParam & 0xFFFF }
    , point{ TranslatePoint(lParam) }
{
}

MouseButton
MouseEvent::
getButton() const
{
    return button;
}

const POINT
MouseEvent::
getPoint() const
{
    return point;
}

bool
MouseEvent::
isCtrlKeyDown() const
{
    return (combination & MK_CONTROL) != 0;
}

bool
MouseEvent::
isShiftKeyDown() const
{
    return (combination & MK_SHIFT) != 0;
}

MouseButton
TranslateButton(UINT message)
{
    switch (message)
    {
    case WM_LBUTTONUP:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
        return MouseButton::LeftButton;
    case WM_RBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
        return MouseButton::RightButton;
    case WM_MBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
        return MouseButton::MiddleButton;
    default:
        return MouseButton::UnknownButton;
    }
}

POINT
TranslatePoint(LPARAM lParam)
{
    POINT point;
    point.x = GET_X_LPARAM(lParam);
    point.y = GET_Y_LPARAM(lParam);
    return point;
}
