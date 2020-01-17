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
    virtual LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam) = 0;
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
    LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam);
};

class MouseEventListener
    : public EventListener
{
public:
    MouseEventListener(const EventHandler& hdlr, HWND hWnd);
    virtual ~MouseEventListener();
    LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam);
protected:
    LRESULT OnClick(const MouseEvent);
};

class WindowEventListener
    : public EventListener
{
public:
    WindowEventListener(const EventHandler& hdlr, HWND hWnd);
    virtual ~WindowEventListener();
    LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam);
protected:
    virtual LRESULT OnCreate();
    virtual LRESULT OnClose();
    virtual LRESULT OnDestroy();
    virtual LRESULT OnResize(const int& newWidth, const int& newHeight);
    virtual LRESULT OnActivate(bool state);
    virtual LRESULT OnPaint();
    virtual LRESULT OnTimer();
};
