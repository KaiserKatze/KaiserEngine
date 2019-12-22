#include "stdafx.h"
#include "EventManager.h"

EventManager::
EventManager(const HWND& hWnd)
{
    this->AddListener(new WindowEventListener(hWnd));
    this->AddListener(new KeyboardEventListener(hWnd));
    this->AddListener(new MouseEventListener(hWnd));
}

EventManager::
~EventManager()
{
}
