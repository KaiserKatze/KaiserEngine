#pragma once

#include <map>

#include "GLBuffer.h"

class GLVertexArray
{
private:
    GLuint id{ 0 };
    std::map<GLstring, GLBuffer> buffers;

public:
    GLVertexArray();
    GLVertexArray(const GLVertexArray& other);
    ~GLVertexArray();

    const GLuint& GetID() const noexcept;

    void Create();
    void Bind();
    void Unbind();
    void Destroy();

    bool AddBuffer(GLstring name, const GLBuffer& buffer);
    GLBuffer& GetBuffer(GLstring name);

    bool operator==(const GLVertexArray& other) const;
    bool operator!=(const GLVertexArray& other) const;
};
