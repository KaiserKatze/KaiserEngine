#pragma once

#include "stdafx.h"
#include "Event.h"
#include "EventListener.h"

using PEventListener = std::unique_ptr<EventListener>;
using LPEventListener = std::vector<PEventListener>;

class EventHandler
{
public:
    EventHandler();
    virtual ~EventHandler();
    LRESULT TraverseList(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void RemoveAllListeners();
    LPEventListener& GetListeners();
private:
    LPEventListener listeners;
};
