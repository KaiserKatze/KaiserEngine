#pragma once

#include "Windows.h"
#include "MouseEvent.h"
#include "EventHandler.h"

class EventListener
{
public:
    EventListener(const EventHandler& eventHandler, HWND windowHandle);
    EventListener(const EventListener&) = delete;
    virtual ~EventListener();
    virtual LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam) const = 0;
protected:
    const EventHandler& getEventHandler() const;
    HWND getWindowHandle() const;
private:
    const EventHandler& hdlr;
    HWND hWnd;
};

class KeyboardEventListener
    : public EventListener
{
public:
    KeyboardEventListener(const EventHandler& hdlr, HWND hWnd);
    virtual ~KeyboardEventListener();
    LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam) const;
};

class MouseEventListener
    : public EventListener
{
public:
    MouseEventListener(const EventHandler& hdlr, HWND hWnd);
    virtual ~MouseEventListener();
    LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam) const;
protected:
    LRESULT OnClick(const MouseEvent) const;
};

class WindowEventListener
    : public EventListener
{
public:
    WindowEventListener(const EventHandler& hdlr, HWND hWnd);
    virtual ~WindowEventListener();
    LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam) const;
protected:
    virtual LRESULT OnCreate() const;
    virtual LRESULT OnClose() const;
    virtual LRESULT OnDestroy() const;
    virtual LRESULT OnResize(const int& newWidth, const int& newHeight) const;
    virtual LRESULT OnActivate(bool state) const;
    virtual LRESULT OnPaint() const;
    virtual LRESULT OnTimer() const;
};
