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
LoadShader(const std::map<GLenum, std::string&>& shaders)
{
    GLuint pId = glCreateProgram();

    for (auto itr = shaders.begin();
        itr != shaders.end();
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

void
ShaderProgram::
Setup(const std::map<GLenum, std::string&>& shaders,
    const std::vector<std::string&>& attributes,
    std::map<std::string&, GLint>& uniforms)
{
    LoadShader(shaders);

    // get all attribute variable locations
    for (auto itr = attributes.begin();
        itr != attributes.end();
        itr++)
    {
        auto index = std::distance(attributes.begin(), itr);
        BindAttribute(index, itr->c_str());
    }

    LinkProgram();

    // get all uniform variable locations
    for (std::map<std::string&, GLint>::iterator itr = uniforms.begin();
        itr != uniforms.end();
        itr++)
    {
        std::string& uniformName{ itr->first };
        GLint& uniformLocation{ itr->second };
        uniformLocation = GetUniformLocation(uniformName.c_str());
    }
}
