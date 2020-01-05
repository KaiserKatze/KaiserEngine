#pragma once

#include "stdafx.h"
#include "Matrix.h"

class Shader
{
private:
    GLuint id{ 0 };

public:
    Shader(GLstring path, const GLenum& type);
    ~Shader();
    const bool isShader() const;
    const GLuint getID() const;
};

class ShaderProgram
{
private:
    GLuint id{ 0 };

public:
    ShaderProgram();
    ~ShaderProgram();

    void AttachShader(const Shader& shader) const;
    void LoadShader(const std::map<GLenum, GLstring>& shaders);
    void BindAttribute(const GLuint& index, GLstring name) const;
    void LinkProgram() const;
    void ValidateProgram() const;
    const GLint GetUniformLocation(GLstring name) const;

    void Setup(const std::map<GLenum, GLstring>& shaders, const std::vector<GLstring>* attributes, std::map<GLstring, GLint>* uniforms);

    void LoadUniformMatrix(const GLint& location, const Matrix::MatrixQ<float, 2>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::MatrixQ<float, 3>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::MatrixQ<float, 4>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::MatrixQ<double, 2>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::MatrixQ<double, 3>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::MatrixQ<double, 4>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::Matrix<float, 2, 3>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::Matrix<float, 3, 2>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::Matrix<float, 2, 4>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::Matrix<float, 4, 2>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::Matrix<float, 3, 4>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::Matrix<float, 4, 3>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::Matrix<double, 2, 3>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::Matrix<double, 3, 2>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::Matrix<double, 2, 4>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::Matrix<double, 4, 2>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::Matrix<double, 3, 4>& matrix);
    void LoadUniformMatrix(const GLint& location, const Matrix::Matrix<double, 4, 3>& matrix);
};
