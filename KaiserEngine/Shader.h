#pragma once

#include "stdafx.h"

class GLShader
{
private:
    GLenum tp{ 0 };
    GLuint id{ 0 };

public:
    GLShader();
    GLShader(GLstring path, const GLenum& type);
    ~GLShader();
    const bool isShader() const;
    const GLuint getID() const;
    const GLenum getType() const;
};

class GLTexture
{
private:
    GLuint id{ 0 };
public:
    GLTexture();
    ~GLTexture();

    const bool IsTexture() const;
    void ActiveTexture(const GLenum& texture) const;
    void BindTexture(const GLenum& target) const;
};
