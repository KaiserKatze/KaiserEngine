#include "stdafx.h"
#include "GLTexture.h"
#include "loadgl.h"

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
Bind(const GLenum& target) const
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

    glBindTexture(target, this->id);
    {
        std::stringstream ss;
        ss << "glBindTexture("
            << target << ", "
            << this->id
            << ")"
            << std::endl;
        DetectGLError(ss);
    }
}
