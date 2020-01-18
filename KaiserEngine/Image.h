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

    int GetWidth() const;
    int GetHeight() const;
};
