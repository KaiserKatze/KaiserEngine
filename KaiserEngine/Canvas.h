#pragma once

#include "Window.h"
#include "Shader.h"

class MainWindow;

class Canvas
{
private:
    const MainWindow& parent;

public:
    Canvas(const MainWindow& window);
    ~Canvas();

    // Setup viewport
    void setup();

    // Setup viewport
    void setup(const int& width, const int& height);

    // Prepare OpenGL resources
    void prepare();

    // Clear canvas
    void clear() const;

    // Render canvas
    void render() const;

    // Dispose OpenGL resources
    void dispose();
};

