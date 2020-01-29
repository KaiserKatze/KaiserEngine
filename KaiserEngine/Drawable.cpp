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
