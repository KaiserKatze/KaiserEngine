#include "stdafx.h"
#include "Image.h"

Image::
Image()
{
}

Image::
~Image()
{
}

int
Image::
GetWidth() const
{
    return width;
}

int
Image::
GetHeight() const
{
    return height;
}

class FileImage final
    : public Image
{
public:
    void SetWidth(const int& width);
    void SetHeight(const int& height);
    void Open(const std::string& path);
};

void
FileImage::
SetWidth(const int& width)
{
    this->width = width;
}

void
FileImage::
SetHeight(const int& height)
{
    this->height = height;
}
