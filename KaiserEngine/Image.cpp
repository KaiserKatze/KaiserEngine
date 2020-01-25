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

int
Image::
GetBitCount() const
{
    return bitCount;
}

const std::vector<RGBQUAD>&
Image::
GetPixels() const
{
    return pixels;
}

class FileImage final
    : public Image
{
public:
    void SetWidth(const int& width);
    void SetHeight(const int& height);
    void Open(const std::wstring& path);
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
Open(const std::wstring& path)
{
    std::vector<char> buffer;
    std::ifstream file{ path };
    if (!file)
        throw std::runtime_error("Fail to open the image file!");
    file.seekg(0, std::ios::end);
    std::streampos length = file.tellg();
    file.seekg(0, std::ios::beg);
#pragma warning(push)
#pragma warning(disable: 4244)
    buffer.resize(length);
#pragma warning(pop)
    file.read(buffer.data(), length);

    char* cpData = buffer.data();
    // @see: https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
    PBITMAPFILEHEADER pFileHeader{
        reinterpret_cast<PBITMAPFILEHEADER>(cpData)
    };
    char* cpInfo = cpData + sizeof(BITMAPFILEHEADER);
    // @see: https://docs.microsoft.com/en-us/previous-versions//dd183376(v=vs.85)?redirectedfrom=MSDN
    PBITMAPINFOHEADER pInfoHeader{
        reinterpret_cast<PBITMAPINFOHEADER>(cpInfo)
    };
#ifdef _DEBUG
    {
        std::wstringstream wss;
        wss
            << "==============================================================="
            << std::endl
            << "FilePath = "
            << path
            << std::endl;
        OutputDebugStringW(wss.str().c_str());

        std::stringstream ss;
        ss
            << "FileSize = "
            << length
            << std::endl
            << "FileHeader::bfSize = "
            << pFileHeader->bfSize
            << std::endl
            << "FileHeader::bfOffBits = "
            << pFileHeader->bfOffBits
            << std::endl
            << "InfoHeader::biSize = "
            << pInfoHeader->biSize
            << "; Should use ";
        switch (pInfoHeader->biSize)
        {
            case sizeof(BITMAPINFOHEADER) :
                ss << "BITMAPINFOHEADER" << std::endl;
                break;
            case sizeof(BITMAPV4HEADER) :
                ss << "BITMAPV4HEADER" << std::endl;
                break;
            case sizeof(BITMAPV5HEADER) :
                ss << "BITMAPV5HEADER" << std::endl;
                break;
            default:
                ss << "BITMAPCOREHEADER" << std::endl;
                break;
        }
        ss
            << "InfoHeader::biWidth = "
            << pInfoHeader->biWidth
            << std::endl
            << "InfoHeader::biHeight = "
            << pInfoHeader->biHeight
            << std::endl
            << "InfoHeader::biPlanes = "
            << pInfoHeader->biPlanes
            << std::endl
            << "InfoHeader::biBitCount = "
            << pInfoHeader->biBitCount
            << std::endl
            << "InfoHeader::biCompression = "
            << pInfoHeader->biCompression
            << std::endl
            << "InfoHeader::biSizeImage = "
            << pInfoHeader->biSizeImage
            << std::endl
            << "InfoHeader::biXPelsPerMeter = "
            << pInfoHeader->biXPelsPerMeter
            << std::endl
            << "InfoHeader::biYPelsPerMeter = "
            << pInfoHeader->biYPelsPerMeter
            << std::endl
            << "InfoHeader::biClrUsed = "
            << pInfoHeader->biClrUsed
            << std::endl
            << "InfoHeader::biClrImportant = "
            << pInfoHeader->biClrImportant
            << std::endl
            << "==============================================================="
            << std::endl;
        OutputDebugStringA(ss.str().c_str());
    }
#endif

    if (strncmp(reinterpret_cast<char*>(&pFileHeader->bfType), "BM", 2) != 0)
    {
        throw std::runtime_error("Invalid file format: this is not a BMP file, or an unsupported OS/2 BMP file!");
    }

    const int width{ std::abs(pInfoHeader->biWidth) };
    const int height{ std::abs(pInfoHeader->biHeight) };
    this->SetWidth(width);
    this->SetHeight(height);

    if (pInfoHeader->biCompression != 0)
    {
        throw std::runtime_error("Decompression of BMP image is unsupported yet!");
    }

    this->pixels.clear();
    const int biCount = this->bitCount = pInfoHeader->biBitCount;
    switch (biCount)
    {
    case 24:
        break;
    case 32:
        break;
    default:
        // Color table is unsupported yet
        throw std::runtime_error("Unsupported bit count!");
    }

    char* cpBitmap = cpData + pFileHeader->bfOffBits;
    const DWORD bitmapSize{ pInfoHeader->biSizeImage };
    this->pixels.resize(bitmapSize);
    std::copy(
        reinterpret_cast<RGBQUAD*>(cpBitmap),
        reinterpret_cast<RGBQUAD*>(cpBitmap + bitmapSize),
        std::begin(this->pixels)
    );
}

Image
Image::
FromFile(const std::wstring& path)
{
    FileImage image;
    image.Open(path);
    return image;
}
