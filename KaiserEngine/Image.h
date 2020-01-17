#pragma once

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
