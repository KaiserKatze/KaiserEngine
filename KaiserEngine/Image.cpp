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

    // @see: https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
    PBITMAPFILEHEADER pFileHeader{
        reinterpret_cast<PBITMAPFILEHEADER>(buffer.data())
    };
    // @see: https://docs.microsoft.com/en-us/previous-versions//dd183376(v=vs.85)?redirectedfrom=MSDN
    PBITMAPINFOHEADER pInfoHeader{
        reinterpret_cast<PBITMAPINFOHEADER>(buffer.data()
            + sizeof(BITMAPFILEHEADER))
    };

    this->SetWidth(pInfoHeader->biWidth);
    this->SetHeight(pInfoHeader->biHeight);

    if (pInfoHeader->biCompression != 0)
    {
        throw std::runtime_error("Decompression of BMP image is unsupported yet!");
    }
}

Image
Image::
FromFile(const std::string& path)
{
    FileImage image;
    image.Open(path);
    return image;
}
