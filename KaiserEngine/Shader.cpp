#include "stdafx.h"
#include "Shader.h"
#include "loadgl.h"

const std::string slurp(std::ifstream& ifs)
{
    std::stringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

GLShader::
GLShader(GLstring path, const GLenum& type)
{
    if (path == nullptr)
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
        id = shaderId;
    }
    else
    {
        throw std::exception("Fail to create shader!");
    }
}

GLShader::
~GLShader()
{
    if (id) glDeleteShader(id);
}

const bool
GLShader::
isShader() const
{
    return id != 0 && glIsShader(id);
}

const GLuint
GLShader::
getID() const
{
    return id;
}

GLProgram::
GLProgram()
{
    if (GLuint pId = glCreateProgram())
    {
        DetectGLError("glCreateProgram");
        id = pId;
        {
            std::stringstream ss;
            ss << "Generated program ID = " << pId
                << " "
                << std::boolalpha
                << static_cast<bool>(glIsProgram(pId))
                << std::endl;
            OutputDebugStringA(ss.str().c_str());
        }
    }
    else
        throw std::exception("Fail to create program!");
}

GLProgram::
~GLProgram()
{
    if (id) glDeleteProgram(id);
}

void
GLProgram::
UseProgram(const GLProgram* program)
{
    GLuint pId{ 0 };
    if (program) pId = program->getID();
    glUseProgram(pId);
}

void
GLProgram::
AttachShader(const GLShader& shader) const
{
    GLuint sId = shader.getID();
    glAttachShader(getID(), sId);
    DetectGLError("glAttachShader");
}

void
GLProgram::
LoadShader(const std::map<GLenum, GLstring>& shaders)
{
    for (auto itr = shaders.cbegin();
        itr != shaders.cend();
        itr++)
    {
        const GLShader shader(itr->second, itr->first);
        AttachShader(shader);
    }
}

void
GLProgram::
BindAttribute(const GLuint& index, GLstring name) const
{
    glBindAttribLocation(getID(), index, name);
    DetectGLError("glBindAttribLocation");
}

void
GLProgram::
LinkProgram() const
{
    glLinkProgram(getID());
    DetectGLError("glLinkProgram");
}

void
GLProgram::
ValidateProgram() const
{
    glValidateProgram(getID());
    DetectGLError("glValidateProgram");
}

const GLint
GLProgram::
GetUniformLocation(GLstring name) const
{
    GLint result = glGetUniformLocation(getID(), name);
    DetectGLError("glGetUniformLocation");
    return result;
}

const GLuint
GLProgram::
getID() const
{
    return id;
}


void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<float, 2>& matrix)
{
    glUniformMatrix2fv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix2fv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<float, 3>& matrix)
{
    glUniformMatrix3fv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix3fv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<float, 4>& matrix)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix4fv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<double, 2>& matrix)
{
    glUniformMatrix2dv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix2dv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<double, 3>& matrix)
{
    glUniformMatrix3dv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix3dv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<double, 4>& matrix)
{
    glUniformMatrix4dv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix4dv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 2, 3>& matrix)
{
    glUniformMatrix2x3fv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix2x3fv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 3, 2>& matrix)
{
    glUniformMatrix3x2fv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix3x2fv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 2, 4>& matrix)
{
    glUniformMatrix2x4fv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix2x4fv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 4, 2>& matrix)
{
    glUniformMatrix4x2fv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix4x2fv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 3, 4>& matrix)
{
    glUniformMatrix3x4fv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix3x4fv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 4, 3>& matrix)
{
    glUniformMatrix4x3fv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix4x3fv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 2, 3>& matrix)
{
    glUniformMatrix2x3dv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix2x3dv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 3, 2>& matrix)
{
    glUniformMatrix3x2dv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix3x2dv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 2, 4>& matrix)
{
    glUniformMatrix2x4dv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix2x4dv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 4, 2>& matrix)
{
    glUniformMatrix4x2dv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix4x2dv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 3, 4>& matrix)
{
    glUniformMatrix3x4dv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix3x4dv");
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 4, 3>& matrix)
{
    glUniformMatrix4x3dv(location, 1, GL_FALSE, matrix.getData().data());
    DetectGLError("glUniformMatrix4x3dv");
}

void
GLProgram::
Setup(const std::map<GLenum, GLstring>& shaders,
    const std::vector<GLstring>* attributes,
    std::map<GLstring, GLint>* uniforms)
{
    LoadShader(shaders);

    // get all attribute variable locations
    if (attributes != nullptr)
        for (auto itr = attributes->cbegin();
            itr != attributes->cend();
            itr++)
        {
            GLuint index = static_cast<GLuint>(std::distance(attributes->cbegin(), itr));
            const std::string& attributeName{ *itr };
            BindAttribute(index, attributeName.c_str());
        }

    LinkProgram();
    ValidateProgram();

    // get all uniform variable locations
    if (uniforms != nullptr)
        for (auto itr = uniforms->begin();
            itr != uniforms->end();
            itr++)
        {
            const std::string& uniformName{ itr->first };
            GLint& uniformLocation{ itr->second };
            uniformLocation = GetUniformLocation(uniformName.c_str());
        }
}
