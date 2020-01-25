#pragma once

#include "GLShader.h"
#include "Matrix.h"

class GLProgram
{
private:
    GLuint id{ 0 };
    std::map<GLenum, GLShader*> shaders;

public:
    GLProgram();
    ~GLProgram();

    static void UseProgram(const GLProgram& program);
    static void UseProgram();

    void Create();
    void Destroy();
    void AttachShader(GLShader* shader);
    // Create and attach shaders to this program
    void LoadShader(const std::map<GLenum, GLstring>& shaders);
    void BindAttribute(const GLuint& index, GLstring name) const;
    void LinkProgram() const;
    void ValidateProgram() const;
    const GLint GetUniformLocation(GLstring name) const;
    void GetProgramState(const GLenum& pname, GLint* params) const;
    const GLuint GetID() const;

    void Setup(const std::map<GLenum, GLstring>& shaders, const std::vector<GLstring>* const attributes, std::map<GLstring, GLint>* const uniforms);

    // @see: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml
    void LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<float, 2>& matrix) const;
    void LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<float, 3>& matrix) const;
    void LoadUniformMatrix(const GLint& location, const MatrixMath::MatrixQ<float, 4>& matrix) const;
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 2, 3>& matrix) const;
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 3, 2>& matrix) const;
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 2, 4>& matrix) const;
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 4, 2>& matrix) const;
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 3, 4>& matrix) const;
    void LoadUniformMatrix(const GLint& location, const MatrixMath::Matrix<float, 4, 3>& matrix) const;
};
