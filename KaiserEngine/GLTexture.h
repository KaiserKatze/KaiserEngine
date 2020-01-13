#pragma once

class GLTexture
{
private:
    GLuint id{ 0 };
public:
    GLTexture();
    ~GLTexture();

    const bool IsTexture() const;
    void ActiveTexture(const GLenum& texture) const;
    void BindTexture(const GLenum& target) const;
};
