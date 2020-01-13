#pragma once

#include "Canvas.h"
#include "BaseWindow.h"

class MainWindow
    : public BaseWindow
{
private:
    Canvas* canvas{ nullptr };
    UINT_PTR timerId{ 0 };

public:
    MainWindow(HINSTANCE hInstance);
    ~MainWindow();

    const Canvas* getCanvas() const;
};
