#pragma once

#include <map>

#include "GLNamedObject.h"
#include "GLBuffer.h"

class GLVertexArray
    : public GLNamedObject
{
private:
    std::map<std::string, GLBuffer> buffers;

protected:
    bool AddBuffer(const std::string& name, const GLBuffer& buffer);

public:
    GLVertexArray();
    GLVertexArray(const GLVertexArray& other);
    ~GLVertexArray();

    void Create();
    void Bind();
    void Unbind();
    void Destroy();

    GLBuffer& GetBuffer(const std::string& name);
    GLBuffer& CreateBuffer(const std::string& name);

    bool operator==(const GLVertexArray& other) const;
};
