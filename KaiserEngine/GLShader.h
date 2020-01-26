#pragma once

#include "stdafx.h"

class GLShader
{
private:
    GLenum tp{ 0 };
    GLuint id{ 0 };

public:
    GLShader();
    GLShader(const std::string& path, const GLenum& type);
    ~GLShader();
    const bool IsShader() const;
    const GLuint GetID() const;
    const GLenum GetType() const;
};
