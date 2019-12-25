#pragma once

#include "stdafx.h"
#include "Event.h"
#include "EventHandler.h"

class EventManager
    : public EventHandler
{
public:
    EventManager();
    ~EventManager();
    void Setup(const HWND& hWnd);
    void Dispose();
};

