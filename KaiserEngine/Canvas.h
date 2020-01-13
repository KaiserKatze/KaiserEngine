#pragma once

#include "AbstractWindow.h"
#include "Shader.h"

class Canvas
{
private:
    const AbstractWindow& parent;
    std::map<GLstring, GLProgram> programs;

public:
    Canvas(const AbstractWindow& window);
    ~Canvas();

    const AbstractWindow& GetParent() const;

    // Setup viewport
    void Setup();

    // Setup viewport
    void Setup(const int& width, const int& height);

    // Prepare OpenGL resources
    void Prepare();

    // Clear canvas
    void Clear() const;

    // Render canvas
    void Render() const;

    // Dispose OpenGL resources
    void Dispose();
};

