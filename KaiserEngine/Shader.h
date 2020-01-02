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

class ShaderProgram
{
private:
    GLuint id{ 0 };

public:
    void LoadShader(const std::map<GLenum, std::string&>& shaders);
    void BindAttribute(GLuint index, GLstring name) const;
    void LinkProgram() const;
    const GLint GetUniformLocation(GLstring name) const;
};
