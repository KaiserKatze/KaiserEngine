#include "stdafx.h"
#include "EventHandler.h"

EventHandler::
EventHandler()
{
}

EventHandler::
~EventHandler()
{
}

LRESULT
EventHandler::
TraverseList(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    for (auto itr = listeners.begin();
        itr != listeners.end();
        itr++)
    {
        itr->get()->Handle(message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void
EventHandler::
AddListener(EventListener* listener)
{
    listeners.push_back(SPEventListener(listener));
}

void
EventHandler::
RemoveAllListeners()
{
    listeners.clear();
}
