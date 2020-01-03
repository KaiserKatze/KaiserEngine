#pragma once

#include "Window.h"

class MainWindow;

class Canvas
{
private:
    const MainWindow& parent;

public:
    Canvas(const MainWindow& window);
    ~Canvas();

    // Setup viewport
    void setup() const;

    // Setup viewport
    void setup(const int& width, const int& height) const;

    // Prepare OpenGL resources
    void prepare() const;

    // Clear canvas
    void clear() const;

    // Render canvas
    void render() const;

    // Dispose OpenGL resources
    void dispose() const;
};

