#pragma once

#include "stdafx.h"
#include "GLNamedObject.h"

class GLShader
    : public GLNamedObject
{
private:
    GLenum tp{ 0 };

public:
    GLShader();
    GLShader(const std::string& path, const GLenum& type);
    ~GLShader();

    const bool IsShader() const;
    const GLenum GetType() const;
};
