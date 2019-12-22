#pragma once

#include "stdafx.h"
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
    std::list<std::shared_ptr<EventListener>> listeners;
};
