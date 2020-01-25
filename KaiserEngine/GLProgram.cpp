#include "stdafx.h"
#include "GLProgram.h"
#include "loadgl.h"

GLProgram::
GLProgram()
{
    if (GLuint pId = glCreateProgram())
    {
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
    {
        DetectGLError("glCreateProgram()");
        throw std::exception("Fail to create program!");
    }
}

GLProgram::
~GLProgram()
{
    for (auto itr = shaders.begin();
        itr != shaders.end();
        itr++)
    {
        GLShader* shader = itr->second;
        if (shader)
            delete shader;
    }
    shaders.clear();

    if (id)
    {
        glDeleteProgram(id);

        {
            std::stringstream ss;
            ss << "Deleted program ID = " << id
                << std::endl;
            OutputDebugStringA(ss.str().c_str());
        }
    }
    id = 0;
}

void
GLProgram::
UseProgram(const GLProgram& program)
{
    const GLuint& pId{ program.GetID() };
    glUseProgram(pId);
    {
        std::stringstream ss;
        ss << "glUseProgram(" << pId << ")";
        DetectGLError(ss);
    }
}

void
GLProgram::
UseProgram()
{
    glUseProgram(0);
    SuppressGLError();
}

void
GLProgram::
AttachShader(GLShader* shader)
{
    GLuint pId = GetID();
    GLuint sId = shader->getID();
    glAttachShader(pId, sId);
    {
        std::stringstream ss;
        ss << "glAttachShader("
            << pId << ", "
            << sId << ")";
        DetectGLError(ss);
    }
    shaders[shader->getType()] = shader;
}

void
GLProgram::
LoadShader(const std::map<GLenum, GLstring>& shaders)
{
    for (auto itr = shaders.cbegin();
        itr != shaders.cend();
        itr++)
    {
        GLShader* shader = new GLShader(itr->second, itr->first);
        AttachShader(shader);
    }
}

void
GLProgram::
BindAttribute(const GLuint& index, GLstring name) const
{
    const GLuint pId = GetID();
    glBindAttribLocation(pId, index, name);
    {
        std::stringstream ss;
        ss << "glBindAttribLocation("
            << pId << ", "
            << index << ", "
            << name << ")";
        DetectGLError(ss);
    }
}

void
GLProgram::
LinkProgram() const
{
    const GLuint pId = GetID();
    glLinkProgram(pId);
    {
        std::stringstream ss;
        ss << "glLinkProgram(" << pId << ")";
        DetectGLError(ss);
    }
}

void
GLProgram::
ValidateProgram() const
{
    const GLuint pId = GetID();
    glValidateProgram(pId);
    {
        std::stringstream ss;
        ss << "glValidateProgram(" << pId << ")";
        DetectGLError(ss);
    }
}

const GLint
GLProgram::
GetUniformLocation(GLstring name) const
{
    const GLuint pId = GetID();
    GLint result = glGetUniformLocation(pId, name);
    {
        std::stringstream ss;
        ss << "glGetUniformLocation(" << pId << ", " << name << ")";
        DetectGLError(ss);
    }
    return result;
}

const GLuint
GLProgram::
GetID() const
{
    if (id == 0)
        throw std::exception("GLProgram refuses to return 0 as its ID!");
    return id;
}


void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<float, 2>& matrix) const
{
    glUniformMatrix2fv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix2fv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<float, 3>& matrix) const
{
    glUniformMatrix3fv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix3fv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<float, 4>& matrix) const
{
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix4fv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 2, 3>& matrix) const
{
    glUniformMatrix2x3fv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix2x3fv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 3, 2>& matrix) const
{
    glUniformMatrix3x2fv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix3x2fv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 2, 4>& matrix) const
{
    glUniformMatrix2x4fv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix2x4fv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 4, 2>& matrix) const
{
    glUniformMatrix4x2fv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix4x2fv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 3, 4>& matrix) const
{
    glUniformMatrix3x4fv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix3x4fv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 4, 3>& matrix) const
{
    glUniformMatrix4x3fv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix4x3fv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
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
    {
        for (auto itr = attributes->cbegin();
            itr != attributes->cend();
            itr++)
        {
            GLuint index = static_cast<GLuint>(std::distance(attributes->cbegin(), itr));
            GLstring attributeName{ *itr };
            BindAttribute(index, attributeName);
        }
    }

    LinkProgram();
    ValidateProgram();

    // get all uniform variable locations
    if (uniforms != nullptr)
    {
        for (auto itr = uniforms->begin();
            itr != uniforms->end();
            itr++)
        {
            GLstring uniformName{ itr->first };
            GLint& uniformLocation{ itr->second };
            uniformLocation = GetUniformLocation(uniformName);
        }
    }
}
