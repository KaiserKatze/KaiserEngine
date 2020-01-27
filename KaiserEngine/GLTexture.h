#pragma once

#include "GLNamedObject.h"
#include "Image.h"

class GLTexture
    : public GLNamedObject
{
private:
    GLenum target{ 0 };

public:
    GLTexture();
    ~GLTexture();

    void SetTarget(const GLenum& target);
    const GLenum& GetTarget() const;
    const bool IsTexture() const;
    void ActiveTexture(const GLenum& texture) const;
    void Create();
    void Bind();
    void Unbind();
    void Destroy();

    void SetPixelStorage(const GLenum& pname, const GLint& param);
    void SetImage(const Image& image, const GLenum& target, const GLint& level, const GLint& internalFormat, const GLenum& format, const GLenum& type);
    void GenerateMipmap();

    static void GenerateMipmap(const GLenum& target);
};
