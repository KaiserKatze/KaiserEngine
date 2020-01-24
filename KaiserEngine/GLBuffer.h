#pragma once

#include <map>

// Routines:
// GLBuffer* buffer = new GLBuffer();
// buffer->SetTarget(target);
// buffer->SetUsage(usage);
// buffer->Bind();
// buffer->SetData(size, data);
// // Do something else
// buffer->Unbind();
class GLBuffer
{
private:
    GLuint id{ 0 };
    GLenum target{ 0 };
    GLenum usage{ 0 };
    GLsizeiptr count{ 0 };
    bool isBound{ false };
    bool isFilled{ false };

public:
    GLBuffer();
    GLBuffer(const GLBuffer& other);
    ~GLBuffer();

    const GLuint& GetID() const noexcept;
    void SetTarget(const GLenum& target) noexcept;
    const GLenum& GetTarget() const noexcept;
    void SetUsage(const GLenum& usage) noexcept;
    const GLenum& GetUsage() const noexcept;
    const GLsizeiptr& GetCount() const noexcept;

    void Create();
    void Bind();
    void Unbind();
    void Destroy();
    void SetData(const GLsizeiptr& size, const void* data);

    bool operator==(const GLBuffer& other) const;
    bool operator!=(const GLBuffer& other) const;
};
