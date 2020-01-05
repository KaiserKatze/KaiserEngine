#pragma once

#include "stdafx.h"

class Shader
{
private:
    GLuint id{ 0 };

public:
    Shader(GLstring path, const GLenum& type);
    ~Shader();
    const bool isShader() const;
    const GLuint getID() const;
};

class ShaderProgram
{
private:
    GLuint id{ 0 };

public:
    ShaderProgram();
    ~ShaderProgram();

    void LoadShader(const std::map<GLenum, GLstring>& shaders);
    void BindAttribute(const GLuint& index, GLstring name) const;
    void LinkProgram() const;
    void ValidateProgram() const;
    const GLint GetUniformLocation(GLstring name) const;

    void Setup(const std::map<GLenum, GLstring>& shaders, const std::vector<GLstring>& attributes, std::map<GLstring, GLint>& uniforms);
};
