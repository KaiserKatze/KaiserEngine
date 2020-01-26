#pragma once

#include <gl/GL.h>
#include "GLObject.h"

class GLNamedObject
    : public GLObject
{
protected:
    GLuint id;

public:
    GLNamedObject();
    virtual ~GLNamedObject();

    const GLuint& GetId() const;
};
