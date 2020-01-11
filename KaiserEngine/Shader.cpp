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
            ss << "glCreateShader(" << type << ")";
            DetectGLError(ss);
        }
        {
            std::stringstream ss;
            ss << "Generated shader ID = " << shaderId
                << " "
                << std::boolalpha
                << static_cast<bool>(glIsShader(shaderId))
                << std::endl;
            OutputDebugStringA(ss.str().c_str());
        }
        const GLsizei count = 1;
        const GLchar* string = text.c_str();
        const GLint length = text.length();
        glShaderSource(shaderId, count, &string, &length);
        {
            std::stringstream ss;
            ss << "glShaderSource("
                << shaderId << ", "
                << count << ", "
                << string << ", "
                << length
                << ")";
            DetectGLError(ss);
        }
        glCompileShader(shaderId);
        {
            std::stringstream ss;
            ss << "glCompileShader("
                << shaderId
                << ")";
            DetectGLError(ss);
        }
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
    id = 0;
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
        DetectGLError("glCreateProgram()");
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
    id = 0;
}

void
GLProgram::
UseProgram(const GLProgram* program)
{
    GLuint pId{ 0 };
    if (program) pId = program->getID();
    glUseProgram(pId); // need not to detect gl error
}

void
GLProgram::
AttachShader(const GLShader& shader) const
{
    GLuint pId = getID();
    GLuint sId = shader.getID();
    glAttachShader(pId, sId);
    {
        std::stringstream ss;
        ss << "glAttachShader("
            << pId << ", "
            << sId << ")";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadShader(const std::map<GLenum, GLstring>& shaders) const
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
    const GLuint pId = getID();
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
    const GLuint pId = getID();
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
    const GLuint pId = getID();
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
    const GLuint pId = getID();
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
getID() const
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
LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<double, 2>& matrix) const
{
    glUniformMatrix2dv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix2dv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<double, 3>& matrix) const
{
    glUniformMatrix3dv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix3dv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<double, 4>& matrix) const
{
    glUniformMatrix4dv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix4dv(" << location << ", 1, GL_FALSE, {})";
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
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 2, 3>& matrix) const
{
    glUniformMatrix2x3dv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix2x3dv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 3, 2>& matrix) const
{
    glUniformMatrix3x2dv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix3x2dv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 2, 4>& matrix) const
{
    glUniformMatrix2x4dv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix2x4dv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 4, 2>& matrix) const
{
    glUniformMatrix4x2dv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix4x2dv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 3, 4>& matrix) const
{
    glUniformMatrix3x4dv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix3x4dv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 4, 3>& matrix) const
{
    glUniformMatrix4x3dv(location, 1, GL_FALSE, matrix.getData().data());
    {
        std::stringstream ss;
        ss << "glUniformMatrix4x3dv(" << location << ", 1, GL_FALSE, {})";
        DetectGLError(ss);
    }
}

void
GLProgram::
Setup(const std::map<GLenum, GLstring>& shaders,
    const std::vector<GLstring>* attributes,
    std::map<GLstring, GLint>* uniforms) const
{
    LoadShader(shaders);

    // get all attribute variable locations
    if (attributes != nullptr)
        for (auto itr = attributes->cbegin();
            itr != attributes->cend();
            itr++)
        {
            GLuint index = static_cast<GLuint>(std::distance(attributes->cbegin(), itr));
            GLstring attributeName{ *itr };
            BindAttribute(index, attributeName);
        }

    LinkProgram();
    ValidateProgram();

    // get all uniform variable locations
    if (uniforms != nullptr)
        for (auto itr = uniforms->begin();
            itr != uniforms->end();
            itr++)
        {
            GLstring uniformName{ itr->first };
            GLint& uniformLocation{ itr->second };
            uniformLocation = GetUniformLocation(uniformName);
        }
}

GLTexture::
GLTexture()
{
    glGenTextures(1, &id);
}

GLTexture::
~GLTexture()
{
    if (id == 0) return;
    glDeleteTextures(1, &id);
    id = 0;
}

const bool
GLTexture::
IsTexture() const
{
    return id != 0 && glIsTexture(id);
}

void
GLTexture::
ActiveTexture(const GLenum& texture) const
{
    // texture: TEXTUREi (where i is
    //      [0, max(MAX_TEXTURE_COORDS,
    //      MAX_COMBINED_TEXTURE_IMAGE_UNITS) - 1] )
    switch (texture)
    {
    case (GL_TEXTURE0):
    case (GL_TEXTURE1):
    case (GL_TEXTURE2):
    case (GL_TEXTURE3):
    case (GL_TEXTURE4):
    case (GL_TEXTURE5):
    case (GL_TEXTURE6):
    case (GL_TEXTURE7):
    case (GL_TEXTURE8):
    case (GL_TEXTURE9):
    case (GL_TEXTURE10):
    case (GL_TEXTURE11):
    case (GL_TEXTURE12):
    case (GL_TEXTURE13):
    case (GL_TEXTURE14):
    case (GL_TEXTURE15):
    case (GL_TEXTURE16):
    case (GL_TEXTURE17):
    case (GL_TEXTURE18):
    case (GL_TEXTURE19):
    case (GL_TEXTURE20):
    case (GL_TEXTURE21):
    case (GL_TEXTURE22):
    case (GL_TEXTURE23):
    case (GL_TEXTURE24):
    case (GL_TEXTURE25):
    case (GL_TEXTURE26):
    case (GL_TEXTURE27):
    case (GL_TEXTURE28):
    case (GL_TEXTURE29):
    case (GL_TEXTURE30):
    case (GL_TEXTURE31):
        break;
    default:
        throw std::invalid_argument("Invalid argument 'texture'!");
    }
    glActiveTexture(texture);
    {
        std::stringstream ss;
        ss << "glActiveTexture("
            << texture
            << ")";
        DetectGLError(ss);
    }
}

void
GLTexture::
BindTexture(const GLenum& target) const
{
    switch (target)
    {
    case (GL_TEXTURE_1D):
    case (GL_TEXTURE_1D_ARRAY):
    case (GL_TEXTURE_2D):
    case (GL_TEXTURE_2D_ARRAY):
    case (GL_TEXTURE_3D):
    case (GL_TEXTURE_RECTANGLE):
    case (GL_TEXTURE_BUFFER):
    case (GL_TEXTURE_CUBE_MAP):
    case (GL_TEXTURE_CUBE_MAP_ARRAY):
    case (GL_TEXTURE_2D_MULTISAMPLE):
    case (GL_TEXTURE_2D_MULTISAMPLE_ARRAY):
        break;
    default:
        throw std::invalid_argument("Invalid argument 'target'!");
    }
}
