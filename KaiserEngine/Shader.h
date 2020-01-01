#pragma once

#include "stdafx.h"

class Shader
{
private:
    const std::string text;

public:
    Shader(const std::string& path);
    ~Shader();
};

