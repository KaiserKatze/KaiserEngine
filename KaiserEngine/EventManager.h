#pragma once

#include "stdafx.h"
#include "Event.h"
#include "EventHandler.h"
#include "EventListener.h"

class EventManager
    : public EventHandler
{
public:
    EventManager(const HWND& hWnd);
    ~EventManager();
};

