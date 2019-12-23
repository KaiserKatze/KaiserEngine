#pragma once

#include "stdafx.h"
#include "Event.h"
#include "EventHandler.h"

class EventListener abstract
{
public:
    EventListener(const HWND& hWnd);
    EventListener(const EventListener&) = delete;
    ~EventListener();
    virtual LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam) const = 0;
protected:
    const HWND& getWindowHandle() const;
private:
    const HWND& hWnd;
};

class KeyboardEventListener : public EventListener
{
public:
    KeyboardEventListener(const HWND& hWnd);
    LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam) const;
};

class MouseEventListener : public EventListener
{
public:
    MouseEventListener(const HWND& hWnd);
    LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam) const;
protected:
    LRESULT OnClick(const MouseEvent) const;
};

class WindowEventListener : public EventListener
{
public:
    WindowEventListener(const HWND& hWnd);
    LRESULT Handle(UINT message, WPARAM wParam, LPARAM lParam) const;
protected:
    LRESULT OnCreate() const;
    LRESULT OnClose() const;
    LRESULT OnDestroy() const;
    LRESULT OnResize(int newWdth, int newHeight) const;
    LRESULT OnActivate(bool state) const;
    LRESULT OnPaint() const;
    LRESULT OnTimer() const;
};
