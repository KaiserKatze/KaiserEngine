#pragma once

#include "Window.h"

class Canvas
{
private:
    const MainWindow& parent;

public:
    Canvas(const MainWindow& window);
    ~Canvas();

    void prepare() const;
    void clear() const;
    void render() const;
    void dispose() const;
};

