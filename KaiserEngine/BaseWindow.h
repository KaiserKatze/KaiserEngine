#pragma once

#include "Window.h"
#include "EventManager.h"

class BaseWindow
    : public AbstractWindow
    , public EventManager
{
protected:
    BaseWindow()
        : AbstractWindow()
        , EventManager()
    {
    }

    virtual ~BaseWindow()
    {
    }

public:
    LRESULT CALLBACK HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) override
    {
        // @see: https://docs.microsoft.com/zh-cn/windows/win32/winmsg/about-messages-and-message-queues
        return TraverseList(getWindowHandle(), message, wParam, lParam);
    }
};
