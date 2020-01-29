#include "stdafx.h"
#include "Drawable.h"

struct DrawableContext
{
    std::vector<VertexData<float>> vertices;
};

Drawable::
Drawable()
    : context{ new DrawableContext }
{
}

Drawable::
~Drawable()
{
    delete context;
    context = nullptr;
}

const std::vector<VertexData<float>>&
Drawable::
GetVertexData() const
{
    return this->context->vertices;
}
