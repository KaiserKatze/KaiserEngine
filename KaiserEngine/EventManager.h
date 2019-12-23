#pragma once

#include "stdafx.h"
#include "EventHandler.h"

class EventManager
    : public EventHandler
{
public:
    EventManager();
    ~EventManager();
    void Setup(const HWND& hWnd);
};

