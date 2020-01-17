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
        EventListener* listener = itr->get();
        listener->Handle(message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void
EventHandler::
RemoveAllListeners()
{
    listeners.clear();
}

LPEventListener&
EventHandler::
GetListeners()
{
    return listeners;
}
