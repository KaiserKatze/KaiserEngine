#pragma once

class AbstractWindow
{
private:
    HWND hWnd;
    HDC hdc;
    HGLRC hglrc;

    std::atomic_bool isFullscreen;
    std::atomic_bool isResizable;
    std::atomic_bool isWindowActivated;
    std::atomic_bool isWindowClosing;

    RECT GetDisplayRect();
    // used in `RegisterWindowClass()`
    UINT MakeWindowClassStyle();
    // used in `RegisterWindowClass()`
    HBRUSH MakeBackgroundColor();
    // used in `InitWindowInstance()`
    DWORD MakeWindowStyle();

    // Register window class, essential for window creation
    bool RegisterWindowClass(HINSTANCE hInstance, WNDPROC wndproc, LPCWSTR lpClass);
    // Create HWND, aka window handle
    bool InitWindowInstance(HINSTANCE hInstance, HWND parent, LPCWSTR lpClass, LPCWSTR lpTitle, int x, int y, int width, int height, int nCmdShow);

protected:
    AbstractWindow();
    virtual ~AbstractWindow();
    virtual LRESULT CALLBACK HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;
    bool Create(HINSTANCE hInstance, HWND parent, LPCWSTR lpClass, LPCWSTR lpTitle, DWORD dwStyle, DWORD dwExStyle, WNDPROC wndproc, int x, int y, int w, int h, int nCmdShow);

public:

    HWND GetWindowHandle() const;
    void SetWindowHandle(HWND windowHandle);
    HDC GetDeviceContext() const;
    void SetDeviceContext(HDC deviceContext);
    HGLRC GetRenderContext() const;
    void SetRenderContext(HGLRC renderContext);
#if (defined _DEBUG) || ((APP_FULLSCREEN != APP_FULLSCREEN_ALWAYS) && (APP_FULLSCREEN != APP_FULLSCREEN_NEVER))
    bool SetFullscreen(bool isFullscreen);
#endif
#if (defined _DEBUG) || ((APP_RESIZABLE != APP_RESIZABLE_ALWAYS) && (APP_RESIZABLE != APP_RESIZABLE_NEVER))
    bool SetResizable(bool isResizable);
#endif
    void Show(bool show) const;
};
