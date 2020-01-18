#pragma once

#include <string>

class Image
{
protected:
    int width{ 0 };
    int height{ 0 };

public:
    Image();
    virtual ~Image();

    // The width of the image, in pixels
    virtual int GetWidth() const;
    // The height of the image, in pixels
    virtual int GetHeight() const;
};
