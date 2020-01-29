#pragma once

#include "GLShader.h"
#include "GLNamedObject.h"
#include "Matrix.h"

class GLProgram
    : public GLNamedObject
{
private:
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
    void LoadShader(const std::map<GLenum, std::string>& shaders);
    void BindAttribute(const GLuint& index, const std::string& name) const;
    void LinkProgram() const;
    void ValidateProgram() const;
    const GLint GetUniformLocation(const std::string& name) const;
    void GetProgramState(const GLenum& pname, GLint* params) const;

    void Setup(const std::map<GLenum, std::string>& shaders, const std::vector<std::string>* const attributes, std::map<std::string, GLint>* const uniforms);

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
