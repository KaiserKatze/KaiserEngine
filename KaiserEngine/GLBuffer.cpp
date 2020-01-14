#include "stdafx.h"
#include "GLBuffer.h"
#include "loadgl.h"

GLBuffer::
GLBuffer()
{
}

GLBuffer::
GLBuffer(const GLBuffer& other)
    : id{ other.id }
    , target{ other.target }
    , usage{ other.usage }
{
}

GLBuffer::
~GLBuffer()
{
}

const GLuint&
GLBuffer::
GetID() const noexcept
{
    return id;
}

void
GLBuffer::
SetTarget(const GLenum& target) noexcept
{
    this->target = target;
}

const GLenum&
GLBuffer::
GetTarget() const noexcept
{
    return target;
}

void
GLBuffer::
SetUsage(const GLenum& usage) noexcept
{
    this->usage = usage;
}

const GLenum&
GLBuffer::
GetUsage() const noexcept
{
    return usage;
}

void
GLBuffer::
Create()
{
    glGenBuffers(1, &(this->id));
    {
        std::stringstream ss;
        ss << "glGenBuffers(1, &(this->id))"
            << std::endl;
        DetectGLError(ss);
    }
}

void
GLBuffer::
Bind()
{
    if (this->isBound)
        throw std::runtime_error("This buffer is already bound!");

    glBindBuffer(this->target, this->id);
    {
        std::stringstream ss;
        ss << "glBindBuffer("
            << this->target << ", "
            << this->id
            << ")"
            << std::endl;
        DetectGLError(ss);
    }

    this->isBound = true;
}

void
GLBuffer::
Unbind()
{
    if (!this->isBound)
        throw std::runtime_error("This buffer is not bound yet!");

    glBindBuffer(this->target, 0);
    SuppressGLError();

    this->isBound = false;
}

void
GLBuffer::
Destroy()
{
    if (this->id == 0) return;

    glDeleteBuffers(1, &this->id);
    {
        std::stringstream ss;
        ss << "glDeleteBuffers(1, "
            << this->id
            << ")"
            << std::endl;
        DetectGLError(ss);
    }
    this->id = 0;
}

void
GLBuffer::
SetData(const GLsizeiptr& size, const void* data)
{
    if (!this->isBound)
        throw std::runtime_error("This buffer is not bound yet!");

    glBufferData(this->target, size, data, this->usage);
    {
        std::stringstream ss;
        ss << "glBufferData("
            << this->target << ", "
            << size << ", "
            << data << ", "
            << this->usage
            << ")"
            << std::endl;
        DetectGLError(ss);
    }
}

bool
GLBuffer::
operator==(const GLBuffer& other) const
{
    return this->id == other.id
        && this->target == other.target
        && this->usage == other.usage;
}

bool
GLBuffer::
operator!=(const GLBuffer& other) const
{
    return this->id != other.id
        || this->target != other.target
        || this->usage != other.usage;
}
