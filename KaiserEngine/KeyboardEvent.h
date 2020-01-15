#pragma once

#include <Windows.h>
#include "Event.h"

class KeyboardEvent
    : public Event
{
public:
    KeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam);
    virtual ~KeyboardEvent();
};
