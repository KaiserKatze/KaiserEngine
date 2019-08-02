#pragma once

#include "resource.h"

template <typename WindowType>
class BaseWindow
{
private:
    ATOM RegisterWindowClass(HINSTANCE hInstance, WNDPROC wndproc, LPCWSTR lpClass);
protected:
    HWND hWnd;

    virtual LRESULT CALLBACK BaseWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
    BaseWindow() : hWnd(nullptr) {}

    BOOL Create(
        LPCWSTR lpTitle,
        LPCWSTR lpClass,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int w = CW_USEDEFAULT,
        int h = CW_USEDEFAULT,
        HWND hWndParent = nullptr,
        HMENU hMenu = nullptr
    )
    {

    }
};
