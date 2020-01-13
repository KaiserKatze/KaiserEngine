#pragma once

#include "AbstractWindow.h"
#include "Shader.h"

class Canvas
{
private:
    const AbstractWindow& parent;
    GLProgram program;

public:
    Canvas(const AbstractWindow& window);
    ~Canvas();

    const AbstractWindow& getParent() const;

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

