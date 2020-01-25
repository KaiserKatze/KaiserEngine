#include "stdafx.h"
#include "GLProgram.h"
#include "loadgl.h"

GLProgram::
GLProgram()
{
}

GLProgram::
~GLProgram()
{
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
Create()
{
    if (GLuint pId{ glCreateProgram() })
    {
        this->id = pId;
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
        throw std::runtime_error("Fail to create program!");
    }
}

void
GLProgram::
Destroy()
{
    const GLuint& pId{ this->GetID() };
    for (auto itr = shaders.begin();
        itr != shaders.end();
        itr++)
    {
        GLShader* shader = itr->second;
        const GLuint& sId{ shader->GetID() };
        glDetachShader(pId, sId);
        if (shader)
            delete shader;
    }
    shaders.clear();

    if (pId)
    {
        glDeleteProgram(pId);

        {
            std::stringstream ss;
            ss << "Deleted program ID = " << pId
                << std::endl;
            OutputDebugStringA(ss.str().c_str());
        }
    }
    this->id = 0;
}

void
GLProgram::
AttachShader(GLShader* shader)
{
    const GLuint& pId{ GetID() };
    const GLuint& sId{ shader->GetID() };
    glAttachShader(pId, sId);
    {
        std::stringstream ss;
        ss << "glAttachShader("
            << pId << ", "
            << sId << ")";
        DetectGLError(ss);
    }
    const GLenum& type{ shader->GetType() };

    auto itr{ this->shaders.find(type) };
    if (itr != this->shaders.end()) // found keyVal
    {
        const GLShader* other{ itr->second };
        if (other == shader) return; // the exaclty same shader is already attached

        throw std::runtime_error("Trying to attach different shaders to the same program!");
    }
    this->shaders[type] = shader;
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
    const GLuint& pId{ this->GetID() };
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
    const GLuint& pId{ this->GetID() };
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
    const GLuint& pId{ this->GetID() };
    glValidateProgram(pId);
    {
        std::stringstream ss;
        ss << "glValidateProgram(" << pId << ")";
        DetectGLError(ss);
    }

    {
        GLint result{ 0 };
        this->GetProgramState(GL_VALIDATE_STATUS, &result);
        if (result == GL_FALSE)
        {
            std::stringstream ss;
            ss << "GLProgram {" << pId
                << "} is invalid!";
            throw std::runtime_error(ss.str());
        }
        else if (result == GL_TRUE)
        {
            std::stringstream ss;
            ss << "GLProgram {" << pId
                << "} is validated: ("
                << "GL_VALIDATE_STATUS -> GL_TRUE)!"
                << std::endl;
            OutputDebugStringA(ss.str().c_str());
        }
    }
}

const GLint
GLProgram::
GetUniformLocation(GLstring name) const
{
    const GLuint& pId{ this->GetID() };
    GLint result = glGetUniformLocation(pId, name);
    {
        std::stringstream ss;
        ss << "glGetUniformLocation(" << pId << ", " << name << ")";
        DetectGLError(ss);
    }
    return result;
}

void
GLProgram::
GetProgramState(const GLenum& pname, GLint* params) const
{
    switch (pname)
    {
    case GL_DELETE_STATUS:
    case GL_LINK_STATUS:
    case GL_VALIDATE_STATUS:
    case GL_INFO_LOG_LENGTH:
    case GL_ATTACHED_SHADERS:
    case GL_ACTIVE_ATOMIC_COUNTER_BUFFERS:
    case GL_ACTIVE_ATTRIBUTES:
    case GL_ACTIVE_ATTRIBUTE_MAX_LENGTH:
    case GL_ACTIVE_UNIFORMS:
    case GL_ACTIVE_UNIFORM_BLOCKS:
    case GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH:
    case GL_ACTIVE_UNIFORM_MAX_LENGTH:
    case GL_PROGRAM_BINARY_LENGTH:
    case GL_TRANSFORM_FEEDBACK_BUFFER_MODE:
    case GL_TRANSFORM_FEEDBACK_VARYINGS:
    case GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH:
    case GL_GEOMETRY_VERTICES_OUT:
    case GL_GEOMETRY_INPUT_TYPE:
    case GL_GEOMETRY_OUTPUT_TYPE:
        break;
    default:
        throw std::invalid_argument("Invalid argument 'pname'!");
    }

    if (params == nullptr)
        throw std::invalid_argument("Invalid argument 'params'!");

    const GLuint& pId{ this->GetID() };
    glGetProgramiv(pId, GL_VALIDATE_STATUS, params);
    {
        std::stringstream ss;
        ss << "glGetProgramiv("
            << pId << ", GL_VALIDATE_STATUS, []"
            << ")";
        DetectGLError(ss);
    }
}

const GLuint
GLProgram::
GetID() const
{
    if (id == 0)
        throw std::runtime_error("GLProgram refuses to return 0 as its ID!");
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
    const std::vector<GLstring>* const attributes,
    std::map<GLstring, GLint>* const uniforms)
{
    // Create and attach shaders
    LoadShader(shaders);

    // get all attribute variable locations
    if (attributes != nullptr)
    {
        for (auto itr = attributes->cbegin();
            itr != attributes->cend();
            itr++)
        {
            ptrdiff_t pdiff{ std::distance(attributes->cbegin(), itr) };
            assert(("Is the size of `attributes` really so large?", pdiff < 0xffffffff));
            GLuint index{ static_cast<GLuint>(pdiff & 0xffffffff) };
            const GLstring& attributeName{ *itr };
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
