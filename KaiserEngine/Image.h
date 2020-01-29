#pragma once

#include <string>
#include <vector>

class Image
{
protected:
    int width{ 0 };
    int height{ 0 };
    int bitCount{ 0 };
    std::vector<RGBQUAD> pixels;

public:
    Image();
    virtual ~Image();

    // The width of the image, in pixels
    virtual int GetWidth() const;
    // The height of the image, in pixels
    virtual int GetHeight() const;
    // The count of bits in a single pixel
    virtual int GetBitCount() const;
    // The pixels, possibly in BGR or BGRA
    virtual const std::vector<RGBQUAD>& GetPixels() const;

    static Image FromFile(const std::wstring& path);
};
