#pragma once

#include "stdafx.h"
#include "Event.h"
#include "EventHandler.h"

class EventListener
{
public:
    EventListener(const EventHandler& eventHandler, const HWND& windowHandle);
    EventListener(const EventListener&) = delete;
    virtual ~EventListener();
    virtual LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam) const = 0;
protected:
    const EventHandler& getEventHandler() const;
    const HWND& getWindowHandle() const;
private:
    const EventHandler& hdlr;
    const HWND& hWnd;
};

class KeyboardEventListener
    : public EventListener
{
public:
    KeyboardEventListener(const EventHandler& hdlr, const HWND& hWnd);
    ~KeyboardEventListener();
    LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam) const;
};

class MouseEventListener
    : public EventListener
{
public:
    MouseEventListener(const EventHandler& hdlr, const HWND& hWnd);
    ~MouseEventListener();
    LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam) const;
protected:
    LRESULT OnClick(const MouseEvent) const;
};

class WindowEventListener
    : public EventListener
{
public:
    WindowEventListener(const EventHandler& hdlr, const HWND& hWnd);
    ~WindowEventListener();
    LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam) const;
protected:
    LRESULT OnCreate() const;
    LRESULT OnClose() const;
    LRESULT OnDestroy() const;
    LRESULT OnResize(int newWidth, int newHeight) const;
    LRESULT OnActivate(bool state) const;
    LRESULT OnPaint() const;
    LRESULT OnTimer() const;
};
