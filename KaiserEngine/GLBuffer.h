#pragma once

#include <map>
#include "GLNamedObject.h"

// Routines:
// GLBuffer* buffer = new GLBuffer();
// buffer->SetTarget(target);
// buffer->SetUsage(usage);
// buffer->Bind();
// buffer->SetData(size, data);
// // Do something else
// buffer->Unbind();
class GLBuffer
    : public GLNamedObject
{
private:
    GLenum target{ 0 };
    GLenum usage{ 0 };
    GLsizeiptr count{ 0 };

public:
    GLBuffer();
    GLBuffer(const GLBuffer& other);
    ~GLBuffer();

    void SetTarget(const GLenum& target);
    const GLenum& GetTarget() const;
    void SetUsage(const GLenum& usage);
    const GLenum& GetUsage() const;
    const GLsizeiptr& GetCount() const;

    void Create();
    void Bind();
    void Unbind();
    void Destroy();
    void SetData(const GLsizeiptr& size, const void* data);

    bool operator==(const GLBuffer& other) const;
};
