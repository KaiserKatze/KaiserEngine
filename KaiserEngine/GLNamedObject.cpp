#include "stdafx.h"
#include "GLNamedObject.h"

GLNamedObject::
GLNamedObject()
    : id{ 0 }
{
}

GLNamedObject::
~GLNamedObject()
{
}

const GLuint&
GLNamedObject::
GetId() const
{
    return this->id;
}
