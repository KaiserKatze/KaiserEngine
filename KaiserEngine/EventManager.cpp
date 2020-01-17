#include "stdafx.h"
#include "EventManager.h"

EventManager::
EventManager()
    : EventHandler()
{
}

EventManager::
~EventManager()
{
    Dispose();
}

void
EventManager::
Setup(HWND hWnd)
{
    if (hWnd)
    {
        LPEventListener& listeners = GetListeners();
        listeners.push_back(std::make_unique<WindowEventListener>(*this, hWnd));
        listeners.push_back(std::make_unique<KeyboardEventListener>(*this, hWnd));
        listeners.push_back(std::make_unique<MouseEventListener>(*this, hWnd));
    }
}

void
EventManager::
Dispose()
{
    RemoveAllListeners();
}
