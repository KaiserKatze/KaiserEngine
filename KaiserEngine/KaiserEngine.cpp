// KaiserEngine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "KaiserEngine.h"
#include "loadgl.h"
#include "UserInput.h"
#include "Event.h"
#include "EventManager.h"
#include "MainWindow.h"

#define MAX_LOADSTRING 100

WPARAM StartMessageLoop(HINSTANCE hInstance)
{
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KAISERENGINE));
    MSG msg;
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
#if 0
        {
            std::stringstream ss;
            ss << "GetMessage from (" << static_cast<void*>(msg.hwnd) << ")!" << std::endl;
            OutputDebugStringA(ss.str().c_str());
        }
#endif
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}

// @see: https://docs.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
#ifdef _DEBUG
    {
        std::wstringstream ss;
        ss
            << "--------------------------------------------" << std::endl
            << "CmdLine: \"" << lpCmdLine << "\"" << std::endl
            << "CmdShow: " << nCmdShow << std::endl
            << "--------------------------------------------" << std::endl
            << std::endl;
        OutputDebugString(ss.str().c_str());
    }
#endif

    int retCode{ 0 };

    try
    {

        MainWindow mainWin(hInstance);

#if APP_FULLSCREEN
        SetCapture(hWnd);
#endif

        retCode = StartMessageLoop(hInstance);

#if APP_FULLSCREEN
        ReleaseCapture();
#endif

    }
    catch (const std::exception& exc)
    {
        std::stringstream ss;
        ss << "!!! EXCEPTION !!! Reason: " << exc.what() << std::endl;
        OutputDebugStringA(ss.str().c_str());
        return -1;
    }

    return retCode;
}
