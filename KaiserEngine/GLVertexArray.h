#pragma once

#include <map>

#include "GLBuffer.h"

class GLVertexArray
{
private:
    GLuint id{ 0 };
    std::map<std::string, GLBuffer> buffers;

protected:
    bool AddBuffer(const std::string& name, const GLBuffer& buffer);

public:
    GLVertexArray();
    GLVertexArray(const GLVertexArray& other);
    ~GLVertexArray();

    const GLuint& GetID() const noexcept;

    void Create();
    void Bind();
    void Unbind();
    void Destroy();

    GLBuffer& GetBuffer(const std::string& name);
    GLBuffer& CreateBuffer(const std::string& name);

    bool operator==(const GLVertexArray& other) const;
    bool operator!=(const GLVertexArray& other) const;
};
