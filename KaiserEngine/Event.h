#pragma once

#include "stdafx.h"

class MouseEvent
{
public:
    MouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
    int getButton() const;
    POINT getPoint() const;
    bool isCtrlKeyDown() const;
    bool isShiftKeyDown() const;
private:
    const int button;
    const int combination;
    const POINT point;
};

enum MouseButton
{
    UnknownButton = 0,
    LeftButton = 1,
    RightButton = 2,
    MiddleButton = 3,
};
