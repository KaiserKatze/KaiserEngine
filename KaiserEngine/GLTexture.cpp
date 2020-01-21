#include "stdafx.h"
#include "GLTexture.h"
#include "loadgl.h"

GLTexture::
GLTexture()
{
}

GLTexture::
~GLTexture()
{
}

void
GLTexture::
SetTarget(const GLenum& target)
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

    this->target = target;
}

const GLenum&
GLTexture::
GetTarget() const
{
    return this->target;
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
Create()
{
    GLuint textureId{ 0 };
    glGenTextures(1, &textureId);
    {
        std::stringstream ss;
        ss << "glGenTextures(1, &id)"
            << std::endl;
        DetectGLError(ss);
    }
    this->id = textureId;
}

void
GLTexture::
Bind()
{
    glBindTexture(this->target, this->id);
    {
        std::stringstream ss;
        ss << "glBindTexture("
            << this->target << ", "
            << this->id
            << ")"
            << std::endl;
        DetectGLError(ss);
    }
}

void
GLTexture::
Unbind()
{
    glBindTexture(this->target, 0);
    SuppressGLError();
}

void
GLTexture::
Destroy()
{
    if (this->id == 0) return;
    glDeleteTextures(1, &id);
    {
        std::stringstream ss;
        ss << "glDeleteTextures(1, &id)"
            << std::endl;
        DetectGLError(ss);
    }
    this->id = 0;
}

void
GLTexture::
SetPixelStorage(const GLenum& pname, const GLint& param)
{
    // @see: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glPixelStore.xhtml

    switch (pname)
    {
    case GL_PACK_SWAP_BYTES:
    case GL_PACK_LSB_FIRST:
    case GL_PACK_ROW_LENGTH:
    case GL_PACK_IMAGE_HEIGHT:
    case GL_PACK_SKIP_PIXELS:
    case GL_PACK_SKIP_ROWS:
    case GL_PACK_SKIP_IMAGES:
    case GL_PACK_ALIGNMENT:
    case GL_UNPACK_SWAP_BYTES:
    case GL_UNPACK_LSB_FIRST:
    case GL_UNPACK_ROW_LENGTH:
    case GL_UNPACK_IMAGE_HEIGHT:
    case GL_UNPACK_SKIP_PIXELS:
    case GL_UNPACK_SKIP_ROWS:
    case GL_UNPACK_SKIP_IMAGES:
    case GL_UNPACK_ALIGNMENT:
        break;
    default:
        {
            std::stringstream ss;
            ss << "Invalid pname: 0x"
                << std::hex
                << pname
                << "!";
            throw std::runtime_error(ss.str());
        }
    } /* switch (pname) */

    glPixelStorei(pname, param);
    {
        std::stringstream ss;
        ss << "glPixelStorei("
            << pname << ", "
            << param << ")"
            << std::endl;
        DetectGLError(ss);
    }

}

void
GLTexture::
SetImage(const Image& image,
    const GLenum& target,
    const GLint& level,
    const GLint& internalFormat,
    const GLenum& format,
    const GLenum& type)
{
    // @see: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml

    switch (target)
    {
    case GL_TEXTURE_2D:
    case GL_PROXY_TEXTURE_2D:
    case GL_TEXTURE_1D_ARRAY:
    case GL_PROXY_TEXTURE_1D_ARRAY:
        break;
    case GL_TEXTURE_RECTANGLE: //
    case GL_PROXY_TEXTURE_RECTANGLE:
        if (level != 0)
            throw std::runtime_error("Invalid level!");
        break;
    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
    case GL_PROXY_TEXTURE_CUBE_MAP:
        break;
    default:
        {
            std::stringstream ss;
            ss << "Invalid target: 0x"
                << std::hex
                << target
                << "!";
            throw std::runtime_error(ss.str());
        }
    } /* switch (target) */

    switch (internalFormat)
    {
    // Base Internal Formats
    case GL_DEPTH_COMPONENT:
    case GL_DEPTH_STENCIL:
    case GL_RED:
    case GL_RG:
    case GL_RGB:
    case GL_RGBA:
    // Sized Internal Formats
    case GL_R8:
    case GL_R8_SNORM:
    case GL_R16:
    case GL_R16_SNORM:
    case GL_RG8:
    case GL_RG8_SNORM:
    case GL_RG16:
    case GL_RG16_SNORM:
    case GL_R3_G3_B2:
    case GL_RGB4:
    case GL_RGB5:
    case GL_RGB8:
    case GL_RGB8_SNORM:
    case GL_RGB10:
    case GL_RGB12:
    case GL_RGB16_SNORM:
    case GL_RGBA2:
    case GL_RGBA4:
    case GL_RGB5_A1:
    case GL_RGBA8:
    case GL_RGBA8_SNORM:
    case GL_RGB10_A2:
    case GL_RGB10_A2UI:
    case GL_RGBA12:
    case GL_RGBA16:
    case GL_SRGB8:
    case GL_SRGB8_ALPHA8:
    case GL_R16F:
    case GL_RG16F:
    case GL_RGB16F:
    case GL_RGBA16F:
    case GL_R32F:
    case GL_RG32F:
    case GL_RGB32F:
    case GL_RGBA32F:
    case GL_R11F_G11F_B10F:
    case GL_RGB9_E5:
    case GL_R8I:
    case GL_R8UI:
    case GL_R16I:
    case GL_R16UI:
    case GL_R32I:
    case GL_R32UI:
    case GL_RG8I:
    case GL_RG8UI:
    case GL_RG16I:
    case GL_RG16UI:
    case GL_RG32I:
    case GL_RG32UI:
    case GL_RGB8I:
    case GL_RGB8UI:
    case GL_RGB16I:
    case GL_RGB16UI:
    case GL_RGB32I:
    case GL_RGB32UI:
    case GL_RGBA8I:
    case GL_RGBA8UI:
    case GL_RGBA16I:
    case GL_RGBA16UI:
    case GL_RGBA32I:
    case GL_RGBA32UI:
    // Compressed
    case GL_COMPRESSED_RED:
    case GL_COMPRESSED_RG:
    case GL_COMPRESSED_RGB:
    case GL_COMPRESSED_RGBA:
    case GL_COMPRESSED_SRGB:
    case GL_COMPRESSED_SRGB_ALPHA:
    case GL_COMPRESSED_RED_RGTC1:
    case GL_COMPRESSED_SIGNED_RED_RGTC1:
    case GL_COMPRESSED_RG_RGTC2:
    case GL_COMPRESSED_SIGNED_RG_RGTC2:
    case GL_COMPRESSED_RGBA_BPTC_UNORM:
    case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM:
    case GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT:
    case GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:
        break;
    default:
        {
            std::stringstream ss;
            ss << "Invalid internal format: 0x"
                << std::hex
                << internalFormat
                << "!";
            throw std::runtime_error(ss.str());
        }
    } /* switch (internalFormat) */

    switch (format)
    {
    case GL_RED:
    case GL_RG:
    case GL_RGB:
    case GL_BGR:
    case GL_RGBA:
    case GL_BGRA:
    case GL_RED_INTEGER:
    case GL_RG_INTEGER:
    case GL_RGB_INTEGER:
    case GL_BGR_INTEGER:
    case GL_RGBA_INTEGER:
    case GL_BGRA_INTEGER:
    case GL_STENCIL_INDEX:
    case GL_DEPTH_COMPONENT:
    case GL_DEPTH_STENCIL:
        break;
    default:
        {
            std::stringstream ss;
            ss << "Invalid format: 0x"
                << std::hex
                << format
                << "!";
            throw std::runtime_error(ss.str());
        }
    } /* switch (format) */

    switch (type)
    {
    case GL_UNSIGNED_BYTE:
    case GL_BYTE:
    case GL_UNSIGNED_SHORT:
    case GL_SHORT:
    case GL_UNSIGNED_INT:
    case GL_INT:
    case GL_HALF_FLOAT:
    case GL_FLOAT:
    case GL_UNSIGNED_BYTE_3_3_2:
    case GL_UNSIGNED_BYTE_2_3_3_REV:
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_5_6_5_REV:
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_4_4_4_4_REV:
    case GL_UNSIGNED_SHORT_5_5_5_1:
    case GL_UNSIGNED_SHORT_1_5_5_5_REV:
    case GL_UNSIGNED_INT_8_8_8_8:
    case GL_UNSIGNED_INT_8_8_8_8_REV:
    case GL_UNSIGNED_INT_10_10_10_2:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
        break;
    default:
        {
            std::stringstream ss;
            ss << "Invalid type: 0x"
                << std::hex
                << type
                << "!";
            throw std::runtime_error(ss.str());
        }
    } /* switch (type) */

    const GLsizei width{ image.GetWidth() };
    const GLsizei height{ image.GetHeight() };
    const GLvoid* pixels{ image.GetPixels().data() };

    if (pixels == nullptr)
    {
        throw std::runtime_error("Invalid pixels: nullptr!");
    }

    glTexImage2D(target, level, internalFormat, width, height, 0, format, type, pixels);
    {
        std::stringstream ss;
        ss << "glTexImage2D("
            << target << ", "
            << level << ", "
            << internalFormat << ", "
            << width << ", "
            << height << ", 0, "
            << format << ", "
            << type << ", "
            << pixels
            << ")"
            << std::endl;
        DetectGLError(ss);
    }
}
