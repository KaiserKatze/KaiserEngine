#include "stdafx.h"
#include "GLVertexArray.h"
#include "loadgl.h"

GLVertexArray::
GLVertexArray()
{
}

GLVertexArray::
GLVertexArray(const GLVertexArray& other)
    : id{ other.id }
{
}

GLVertexArray::
~GLVertexArray()
{
}

const GLuint&
GLVertexArray::
GetID() const noexcept
{
    return this->id;
}

void
GLVertexArray::
Create()
{
    glGenVertexArrays(1, &(this->id));
    {
        std::stringstream ss;
        ss << "glGenVertexArrays(1, &(this->id)))"
            << std::endl;
        DetectGLError(ss);
    }
}

void
GLVertexArray::
Bind()
{
    glBindVertexArray(this->id);
    {
        std::stringstream ss;
        ss << "glBindVertexArray("
            << this->id
            << ")"
            << std::endl;
        DetectGLError(ss);
    }
}

void
GLVertexArray::
Unbind()
{
    glBindVertexArray(0);
    SuppressGLError();
}

void
GLVertexArray::
Destroy()
{
    if (this->id != 0)
    {
        for (auto itr = this->buffers.begin();
            itr != this->buffers.end();
            itr++)
        {
            GLBuffer& entry = itr->second;
            entry.Destroy();
        }
        this->buffers.clear();

        glDeleteVertexArrays(1, &(this->id));
        {
            std::stringstream ss;
            ss << "glDeleteVertexArrays(1, &(this->id)))"
                << std::endl;
            DetectGLError(ss);
        }

        this->id = 0;
    }
}

bool
GLVertexArray::
AddBuffer(GLstring name, const GLBuffer& buffer)
{
    // There is a buffer having the exactly same name with this one in record!
    if (this->buffers.find(name) != this->buffers.end())
        return false;

    this->buffers[name] = buffer;
    return true;
}

GLBuffer&
GLVertexArray::
GetBuffer(GLstring name)
{
    return this->buffers.at(name);
}

bool
GLVertexArray::
operator==(const GLVertexArray& other) const
{
    return this->id == other.id;
}

bool
GLVertexArray::
operator!=(const GLVertexArray& other) const
{
    return this->id != other.id;
}
