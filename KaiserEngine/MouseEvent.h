#pragma once

#include <Windows.h>
#include "Event.h"

enum MouseButton
{
    UnknownButton = 0,
    LeftButton = 1,
    RightButton = 2,
    MiddleButton = 3,
};

class MouseEvent
    : public Event
{
public:
    MouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
    ~MouseEvent();
    MouseButton getButton() const;
    const POINT getPoint() const;
    bool isCtrlKeyDown() const;
    bool isShiftKeyDown() const;
private:
    const MouseButton button;
    const int combination;
    const POINT point;
};
