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
    AddListener(new WindowEventListener(hWnd));
    AddListener(new KeyboardEventListener(hWnd));
    AddListener(new MouseEventListener(hWnd));
}
