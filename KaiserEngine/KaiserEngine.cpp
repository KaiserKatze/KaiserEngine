// KaiserEngine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "KaiserEngine.h"
#include "loadgl.h"
#include "UserInput.h"
#include "Event.h"
#include "EventManager.h"
#include "Window.h"

#define MAX_LOADSTRING 100

void gl_init(HWND hWnd);
void vao_init();
void vao_exit();
void vao_draw();

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

    FakeWindow fakeWin(hInstance);
    MainWindow mainWin(hInstance);

#if APP_FULLSCREEN
    SetCapture(hWnd);
#endif

    int retCode;
    retCode = StartMessageLoop(hInstance);

#if APP_FULLSCREEN
    ReleaseCapture();
#endif

    return retCode;
}


void kglViewport(int width, int height)
{
    const int trimX = 10;
    const int trimY = 10;

    if (height <= 0) height = 1;

    glViewport(trimX, trimY, width - 2 * trimX, height - 2 * trimY);

    const double aspectRatio = double(width) / double(height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspectRatio, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
}

void OnResize(int width, int height)
{
    kglViewport(width, height);
}

GLuint vbo[4] = { -1,-1,-1,-1 };
GLuint vao[4] = { -1,-1,-1,-1 };
const GLfloat vao_pos[] =
{
    //  x    y    z
    -1.0,-1.0,-1.0,
    +1.0,-1.0,-1.0,
    +1.0,+1.0,-1.0,
    -1.0,+1.0,-1.0,
    -1.0,-1.0,+1.0,
    +1.0,-1.0,+1.0,
    +1.0,+1.0,+1.0,
    -1.0,+1.0,+1.0,
};
const GLfloat vao_col[] =
{
    //  r   g   b
    0.0,0.0,0.0,
    1.0,0.0,0.0,
    1.0,1.0,0.0,
    0.0,1.0,0.0,
    0.0,0.0,1.0,
    1.0,0.0,1.0,
    1.0,1.0,1.0,
    0.0,1.0,1.0,
};
const GLuint vao_ix[] =
{
    0,1,2,3,
    4,5,6,7,
    0,1,5,4,
    1,2,6,5,
    2,3,7,6,
    3,0,4,7,
};
void gl_init(HWND hWnd)
{
    RECT rect = { 0 };
    GetClientRect(hWnd, &rect);
    const int screenWidth = rect.right - rect.left;
    if (screenWidth <= 0)
        throw 1;
    const int screenHeight = rect.bottom - rect.top;
    if (screenHeight <= 0)
        throw 1;

    // @see: http://falloutsoftware.com/tutorials/gl/gl2.htm
    glClearColor(.4f, .6f, .9f, .0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    kglViewport(screenWidth, screenHeight);
}
void vao_init()
{
    GLuint i;
    glGenVertexArrays(4, vao);
    glGenBuffers(4, vbo);
    glBindVertexArray(vao[0]);
    i = 0; // vertex
    glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vao_pos), vao_pos, GL_STATIC_DRAW);
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
    i = 1; // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[i]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vao_ix), vao_ix, GL_STATIC_DRAW);
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
    i = 2; // normal
    i = 3; // color
    glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vao_col), vao_col, GL_STATIC_DRAW);
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    //  glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}
//---------------------------------------------------------------------------
void vao_exit()
{
    glDeleteVertexArrays(4, vao);
    glDeleteBuffers(4, vbo);
}
//---------------------------------------------------------------------------
void vao_draw()
{
    const HDC hdc = wglGetCurrentDC();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao[0]);
    //  glDrawArrays(GL_LINE_LOOP,0,8);                 // lines ... no indices
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);  // indices (choose just one line not both !!!)
    glBindVertexArray(0);

    SwapBuffers(hdc);

    DetectGLError(9);
}
