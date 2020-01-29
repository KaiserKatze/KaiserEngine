#include "stdafx.h"
#include "GLVertexArray.h"
#include "loadgl.h"

GLVertexArray::
GLVertexArray()
{
}

GLVertexArray::
GLVertexArray(const GLVertexArray& other)
    : GLNamedObject(other.id)
{
}

GLVertexArray::
~GLVertexArray()
{
}

void
GLVertexArray::
Create()
{
    if (this->id) return; // GLVertexArray is already created!

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
AddBuffer(const std::string& name, const GLBuffer& buffer)
{
    // There is a buffer having the exactly same name with this one in record!
    if (this->buffers.find(name) != this->buffers.end())
        return false;

    this->buffers[name] = buffer;
    return true;
}

GLBuffer&
GLVertexArray::
GetBuffer(const std::string& name)
{
    return this->buffers.at(name);
}

GLBuffer&
GLVertexArray::
CreateBuffer(const std::string& name)
{
    this->AddBuffer(name, GLBuffer());
    return this->GetBuffer(name);
}

bool
GLVertexArray::
operator==(const GLVertexArray& other) const
{
    return this->GLNamedObject::operator==(other)
        || this->id == other.id;
}
