#pragma once

#include "stdafx.h"
#include "Matrix.h"

class GLShader
{
private:
    GLuint id{ 0 };

public:
    GLShader(GLstring path, const GLenum& type);
    ~GLShader();
    const bool isShader() const;
    const GLuint getID() const;
};

class GLProgram
{
private:
    GLuint id{ 0 };

public:
    GLProgram();
    ~GLProgram();

    static void UseProgram(const GLProgram* program=nullptr);

    void AttachShader(const GLShader& shader) const;
    void LoadShader(const std::map<GLenum, GLstring>& shaders);
    void BindAttribute(const GLuint& index, GLstring name) const;
    void LinkProgram() const;
    void ValidateProgram() const;
    const GLint GetUniformLocation(GLstring name) const;
    const GLuint getID() const;

    void Setup(const std::map<GLenum, GLstring>& shaders, const std::vector<GLstring>* attributes, std::map<GLstring, GLint>* uniforms);

    // @see: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml
    void LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<float, 2>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<float, 3>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<float, 4>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<double, 2>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<double, 3>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<double, 4>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 2, 3>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 3, 2>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 2, 4>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 4, 2>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 3, 4>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 4, 3>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 2, 3>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 3, 2>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 2, 4>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 4, 2>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 3, 4>& matrix);
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<double, 4, 3>& matrix);
};

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
