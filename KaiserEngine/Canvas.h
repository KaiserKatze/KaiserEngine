#pragma once

#include "Window.h"

class Canvas
{
private:
    const MainWindow& parent;
public:
    Canvas(const MainWindow& window);
    ~Canvas();
    void Prepare();
    void Paint();
    void Dispose();
};

