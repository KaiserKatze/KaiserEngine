#pragma once

#include "stdafx.h"
#include "Event.h"
#include "EventListener.h"

using PEventListener = std::unique_ptr<EventListener>;
using LPEventListener = std::vector<PEventListener>;

class EventHandler
{
public:
    EventHandler();
    virtual ~EventHandler();
    LRESULT TraverseList(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void RemoveAllListeners();
    LPEventListener& GetListeners();

    template <typename WindowType>
    const WindowType* AsWindow() const;

private:
    LPEventListener listeners;
};

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
