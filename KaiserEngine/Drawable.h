#pragma once

#include <vector>
#include "VertexData.h"

struct DrawableContext;

class Drawable
{
private:
    const DrawableContext* context;

public:
    const std::vector<VertexData<float>>& GetVertexData() const;
    std::vector<VertexData<float>>& GetVertexData();
};
