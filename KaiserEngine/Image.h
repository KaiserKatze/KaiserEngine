#pragma once

#include <string>
#include <vector>

struct PixelBGR
{
    unsigned char b;    // blue
    unsigned char g;    // green
    unsigned char r;    // red
};

struct PixelBGRA
    : PixelBGR
{
    unsigned char a;    // alpha
};

class Image
{
protected:
    int width{ 0 };
    int height{ 0 };
    std::vector<unsigned char> pixels;

public:
    Image();
    virtual ~Image();

    // The width of the image, in pixels
    virtual int GetWidth() const;
    // The height of the image, in pixels
    virtual int GetHeight() const;
    // The pixels, possibly in BGR or BGRA
    virtual const std::vector<unsigned char>& GetPixels() const;

    static Image FromFile(const std::string& path);
};
