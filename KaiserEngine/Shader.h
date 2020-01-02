#pragma once

#include "stdafx.h"

class Shader
{
private:
    GLuint id{ 0 };

public:
    Shader(const std::string& path, const GLenum& type);
    ~Shader();
    const GLuint getID() const;
};

