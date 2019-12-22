#pragma once

#include "stdafx.h"

class EventManager
    : public EventHandler
{
public:
    EventManager(const HWND& hWnd);
};

