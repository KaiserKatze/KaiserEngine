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
    if (path.empty())
        throw std::invalid_argument("Invalid argument 'path'!");
    switch (type)
    {
    case GL_VERTEX_SHADER:
    case GL_FRAGMENT_SHADER:
        break;
    default:
        throw std::invalid_argument("Invalid argument 'type'!");
    }

    const std::string text = slurp(std::ifstream{ path });
    if (GLuint shaderId = glCreateShader(type))
    {
        {
            std::stringstream ss;
            ss << "Generated shader ID = " << shaderId
                << " "
                << std::boolalpha
                << static_cast<bool>(glIsShader(shaderId))
                << std::endl;
            OutputDebugStringA(ss.str().c_str());
        }
        DetectGLError("glCreateShader");
        const GLsizei count = 1;
        const GLchar* string = text.c_str();
        const GLint length = text.length();
        glShaderSource(shaderId, count, &string, &length);
        DetectGLError("glShaderSource");
        glCompileShader(shaderId);
        DetectGLError("glCompileShader");
        GLint status{ 0 };
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
        DetectGLError("glGetShaderiv");
        if (status == GL_FALSE)
        {
            ErrorExit(L"glCompileShader");
            return;
        }
        id = shaderId;
    }
    else
    {
        throw std::exception("Fail to create shader as required!");
    }
}

Shader::
~Shader()
{
    if (id) glDeleteShader(id);
}

const bool
Shader::
isShader() const
{
    return id != 0 && glIsShader(id);
}

const GLuint
Shader::
getID() const
{
    return id;
}

void
ShaderProgram::
LoadShader(const std::map<GLenum, std::string>& shaders)
{
    GLuint pId = glCreateProgram();

    for (auto itr = shaders.cbegin();
        itr != shaders.cend();
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
    return glGetUniformLocation(id, name);
}

void
ShaderProgram::
Setup(const std::map<GLenum, std::string>& shaders,
    const std::vector<std::string>& attributes,
    std::map<std::string, GLint>& uniforms)
{
    LoadShader(shaders);

    // get all attribute variable locations
    for (auto itr = attributes.cbegin();
        itr != attributes.cend();
        itr++)
    {
        ptrdiff_t index = std::distance(attributes.cbegin(), itr);
        const std::string& attributeName{ *itr };
        BindAttribute(static_cast<GLuint>(index), attributeName.c_str());
    }

    LinkProgram();

    // get all uniform variable locations
    for (auto itr = uniforms.begin();
        itr != uniforms.end();
        itr++)
    {
        const std::string& uniformName{ itr->first };
        GLint& uniformLocation{ itr->second };
        uniformLocation = GetUniformLocation(uniformName.c_str());
    }
}
