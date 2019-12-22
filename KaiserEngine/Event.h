#pragma once

#include "stdafx.h"

enum MouseButton
{
    UnknownButton = 0,
    LeftButton = 1,
    RightButton = 2,
    MiddleButton = 3,
};

class MouseEvent
{
public:
    MouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
    MouseButton getButton() const;
    POINT getPoint() const;
    bool isCtrlKeyDown() const;
    bool isShiftKeyDown() const;
private:
    const MouseButton button;
    const int combination;
    const POINT point;
};
