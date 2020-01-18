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

void
FileImage::
Open(const std::string& path)
{
    std::vector<char> buffer;
    std::ifstream file{ path };
    if (!file)
        throw std::runtime_error("Fail to open the image file!");
    file.seekg(0, std::ios::end);
    std::streampos length = file.tellg();
    file.seekg(0, std::ios::beg);
    buffer.resize(length);
    file.read(buffer.data(), length);

    PBITMAPFILEHEADER pFileHeader{
        reinterpret_cast<PBITMAPFILEHEADER>(buffer.data())
    };
    PBITMAPINFOHEADER pInfoHeader{
        reinterpret_cast<PBITMAPINFOHEADER>(buffer.data()
            + sizeof(BITMAPFILEHEADER))
    };

    this->SetWidth(pInfoHeader->biWidth);
    this->SetHeight(pInfoHeader->biHeight);
}
