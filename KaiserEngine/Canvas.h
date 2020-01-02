#pragma once

#include "Window.h"

class Canvas
{
private:
    const MainWindow& parent;

public:
    Canvas(const MainWindow& window);
    ~Canvas();

    const RECT GetClientArea() const;

    void Prepare() const;
    void Render() const;
    void Dispose() const;
};

