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

template <typename WindowType>
const WindowType*
EventHandler::
AsWindow() const
{
    // (WindowType) -> BaseWindow -> EventManager -> EventHandler
    // downcast pointer to `eventHandler`
    // from `const EventHandler*` to `const WindowType*`
    return dynamic_cast<const WindowType*>(this);
}
