#pragma once

#include <memory>
#include <vector>
#include "VertexData.h"

struct DrawableContext;

class Drawable
{
private:
    std::unique_ptr<DrawableContext> context;

public:
    Drawable();
    ~Drawable();

    const std::vector<VertexData<float>>& GetVertexData() const;
    std::vector<VertexData<float>>& GetVertexData();
};
