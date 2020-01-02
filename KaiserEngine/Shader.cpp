#include "stdafx.h"
#include "Shader.h"
#include "loadgl.h"

const std::string slurp(std::ifstream& ifs)
{
    std::stringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

Shader::
Shader(const std::string& path, const GLenum& type)
{
    const std::string text = slurp(std::ifstream{ path });
    if (GLuint shaderId = glCreateShader(type))
    {
        const GLsizei count = 1;
        const GLchar* string = text.c_str();
        const GLint length = text.length();
        glShaderSource(shaderId, count, &string, &length);
        glCompileShader(shaderId);
        GLint status{ 0 };
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            ErrorExit(L"glCompileShader");
            return;
        }
        id = shaderId;
    }
}

Shader::
~Shader()
{
}

const GLuint
Shader::
getID() const
{
    return id;
}

void
ShaderProgram::
LoadShader(const std::map<GLenum, std::string&>& map)
{
    GLuint pId = glCreateProgram();

    for (auto itr = map.begin();
        itr != map.end();
        itr++)
    {
        const Shader shader(itr->second, itr->first);
        GLuint sId = shader.getID();
        glAttachShader(pId, sId);
    }

    id = pId;
}

void
ShaderProgram::
BindAttribute(GLuint index, GLstring name) const
{
    glBindAttribLocation(id, index, name);
}

void
ShaderProgram::
LinkProgram() const
{
    glLinkProgram(id);
    glValidateProgram(id);
}

const GLint
ShaderProgram::
GetUniformLocation(GLstring name) const
{
    glGetUniformLocation(id, name);
}
