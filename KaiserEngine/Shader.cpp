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
