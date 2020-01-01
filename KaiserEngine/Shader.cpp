#include "stdafx.h"
#include "Shader.h"
#include "loadgl.h"

const std::string slurp(std::ifstream& ifs)
{
    std::stringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

Shader::
Shader(const std::string& path)
    : text{ slurp(std::ifstream{path}) }
{
}

Shader::
~Shader()
{
}
