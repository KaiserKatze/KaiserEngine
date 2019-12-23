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
    AddListener(new WindowEventListener(this, hWnd));
    AddListener(new KeyboardEventListener(this, hWnd));
    AddListener(new MouseEventListener(this, hWnd));
}
