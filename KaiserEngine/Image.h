#pragma once

#include <string>

class Image
{
private:
    int width;
    int height;

public:
    Image();
    virtual ~Image();

    int GetWidth() const;
    int GetHeight() const;
};
