#include "stdafx.h"
#include "Drawable.h"

struct DrawableContext
{
    std::vector<VertexData<float>> vertices;
};

Drawable::
Drawable()
    : context{ std::make_unique<DrawableContext>() }
{
}

Drawable::
~Drawable()
{
}

const std::vector<VertexData<float>>&
Drawable::
GetVertexData() const
{
    return this->context->vertices;
}

std::vector<VertexData<float>>&
Drawable::
GetVertexData()
{
    return this->context->vertices;
}
