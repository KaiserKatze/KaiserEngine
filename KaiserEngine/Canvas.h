#pragma once

#include "AbstractWindow.h"
#include "GLShader.h"
#include "GLProgram.h"
#include "GLVertexArray.h"
#include "GLTexture.h"
#include "Drawable.h"

class Canvas
{
private:
    const AbstractWindow& parent;
    std::map<std::string, GLProgram> programs;
    std::map<std::string, GLVertexArray> vaos;
    std::map<std::string, GLTexture> texs;

    GLProgram& GetProgram(const std::string& name);
    GLVertexArray& GetVertexArray(const std::string& name);
    GLTexture& GetTexture(const std::string& name);

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
    void Clear();

    // Render canvas
    void Render();

    // Dispose OpenGL resources
    void Dispose();

    GLVertexArray& CreateVertexArray(const std::string& name);
    GLProgram& CreateProgram(const std::string& name);
    GLTexture& CreateTexture(const std::string& name);
};

