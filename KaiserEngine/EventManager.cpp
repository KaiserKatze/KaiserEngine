#include "stdafx.h"
#include "EventManager.h"

EventManager::
EventManager()
{
}

EventManager::
~EventManager()
{
    this->RemoveAllListeners();
}

void
EventManager::
Setup(const HWND& hWnd)
{
    LPEventListener& listeners = GetListeners();
    listeners.push_back(std::make_unique<WindowEventListener>(this, hWnd));
    listeners.push_back(std::make_unique<KeyboardEventListener>(this, hWnd));
    listeners.push_back(std::make_unique<MouseEventListener>(this, hWnd));
}
