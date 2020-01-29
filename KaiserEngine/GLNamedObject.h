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
    GLNamedObject(const GLuint& id);
    virtual ~GLNamedObject();

    const GLuint& GetID() const;

    virtual bool operator==(const GLNamedObject& other) const;
    virtual bool operator!=(const GLNamedObject& other) const;
};
