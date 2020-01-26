#include "stdafx.h"
#include "GLNamedObject.h"

GLNamedObject::
GLNamedObject()
    : id{ 0 }
{
}

GLNamedObject::
GLNamedObject(const GLuint& id)
    : id{ id }
{
}

GLNamedObject::
~GLNamedObject()
{
}

const GLuint&
GLNamedObject::
GetID() const
{
    return this->id;
}
