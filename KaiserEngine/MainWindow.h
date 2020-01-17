#pragma once

#include "Canvas.h"
#include "BaseWindow.h"

class MainWindow
    : public BaseWindow
{
private:
    Canvas canvas;
    UINT_PTR timerId{ 0 };

public:
    MainWindow(HINSTANCE hInstance);
    ~MainWindow();

    Canvas& getCanvas();
};
