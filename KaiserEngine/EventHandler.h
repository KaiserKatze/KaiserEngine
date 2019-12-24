#pragma once

#include "stdafx.h"
#include "Event.h"
#include "EventListener.h"

class EventHandler
{
public:
    EventHandler();
    ~EventHandler();
    LRESULT TraverseList(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void AddListener(EventListener* listener);
    void RemoveAllListeners();
private:
    std::vector<std::unique_ptr<EventListener>> listeners;
};
