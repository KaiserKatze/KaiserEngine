#pragma once

#include "stdafx.h"
#include "EventListener.h"

class EventHandler
{
public:
    EventHandler();
    ~EventHandler();

    using SPEventListener = std::shared_ptr<EventListener>;

    LRESULT TraverseList(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void AddListener(EventListener* listener);
    void RemoveAllListeners();
private:
    std::list<SPEventListener> listeners;
};
