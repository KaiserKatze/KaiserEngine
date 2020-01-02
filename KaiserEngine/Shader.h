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
    ShaderProgram() {}
    ~ShaderProgram() {}

    void LoadShader(const std::map<GLenum, std::string&>& shaders);
    void BindAttribute(GLuint index, GLstring name) const;
    void LinkProgram() const;
    const GLint GetUniformLocation(GLstring name) const;

    void Setup(const std::map<GLenum, std::string&>& shaders, const std::vector<std::string&>& attributes, std::map<std::string&, GLint>& uniforms);
};
