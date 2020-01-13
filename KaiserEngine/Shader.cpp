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
GLShader()
{
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
                << count << ", \"...\", "
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
        tp = type;
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
    if (id)
    {
        glDeleteShader(id);

        {
            std::stringstream ss;
            ss << "Deleted shader ID = " << id
                << std::endl;
            OutputDebugStringA(ss.str().c_str());
        }
    }
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

const GLenum
GLShader::
getType() const
{
    return tp;
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
