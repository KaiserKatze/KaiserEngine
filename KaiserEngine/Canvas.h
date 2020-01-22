#pragma once

#include "AbstractWindow.h"
#include "GLShader.h"
#include "GLProgram.h"
#include "GLVertexArray.h"

class Canvas
{
private:
    const AbstractWindow& parent;
    std::map<GLstring, GLProgram> programs;
    std::map<GLstring, GLVertexArray> vaos;
    std::map<GLstring, GLTexture> texs;

protected:
    bool AddVertexArray(GLstring name, const GLVertexArray& array);
    GLVertexArray& GetVertexArray(GLstring name);

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

    GLVertexArray& CreateVertexArray(GLstring name);
    GLProgram& CreateProgram(GLstring name);
};

