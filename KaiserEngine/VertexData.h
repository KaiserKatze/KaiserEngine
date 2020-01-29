#pragma once

#include <initializer_list>
#include <sstream>
#include <algorithm>
#include <gl/GL.h>

#include "loadgl.h"

template <typename _Ty>
struct VertexData
{
    struct PositionArray
    {
        _Ty x;
        _Ty y;
        _Ty z;
        _Ty w;
    } positions;
    struct ColorArray
    {
        _Ty r;
        _Ty g;
        _Ty b;
        _Ty a;
    } colors;
    struct TextureArray
    {
        _Ty u;
        _Ty v;
    } texture;

    using DataType = _Ty;

    VertexData();
    VertexData(const std::initializer_list<_Ty>& initializerList);
    ~VertexData() {}

    constexpr static int PositionElementCount   = sizeof(PositionArray) / sizeof(_Ty);  /* x, y, z, w */
    constexpr static int ColorElementCount      = sizeof(ColorArray)    / sizeof(_Ty);  /* r, g, b, a */
    constexpr static int TextureElementCount    = sizeof(TextureArray)  / sizeof(_Ty);  /* u, v */

    constexpr static int PositionByteCount      = PositionElementCount * sizeof(DataType);
    constexpr static int ColorByteCount         = ColorElementCount * sizeof(DataType);
    constexpr static int TextureByteCount       = TextureElementCount * sizeof(DataType);

    constexpr static int PositionByteOffset     = 0;
    constexpr static int ColorByteOffset        = PositionByteOffset + PositionByteCount;
    constexpr static int TextureByteOffset      = ColorByteOffset + ColorByteCount;

    static void SetVertexAttribPointer() {}
    static void EnableVertexAttribArray();
    static void DisableVertexAttribArray();
};

template <typename _Ty>
VertexData<_Ty>::
VertexData()
    : positions{ 0, 0, 0, 0 }
    , colors{ 0, 0, 0, 0 }
    , texture{ 0, 0 }
{
}

template <>
VertexData<float>::
VertexData()
    : positions{ 0.0f, 0.0f, 0.0f, 0.0f }
    , colors{ 0.0f, 0.0f, 0.0f, 0.0f }
    , texture{ 0.0f, 0.0f }
{
}

template <typename _Ty>
VertexData<_Ty>::
VertexData(const std::initializer_list<_Ty>& initializerList)
    : VertexData()
{
    _Ty* dataBegin = reinterpret_cast<_Ty*>(&(this->positions));
    size_t size{ std::min<size_t>(initializerList.size(), sizeof(VertexData) / sizeof(_Ty)) };
    const _Ty* sourceBegin{ initializerList.begin() };
    const _Ty* sourceEnd{ sourceBegin + size };
    std::copy(sourceBegin, sourceEnd, dataBegin);
}

template <>
void
VertexData<float>::
SetVertexAttribPointer()
{
    const GLenum type{ GL_FLOAT };
    const GLboolean normalized{ GL_FALSE };
    const GLsizei stride{ sizeof(VertexData) };

    const auto setVAP = [](
        const GLuint & index,
        const GLint & size,
        const GLenum & type,
        const GLboolean & normalized,
        const GLsizei & stride,
        const void* pointer) -> void
    {
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        {
            std::stringstream ss;
            ss << "glVertexAttribPointer("
                << index << ", "
                << size << ", "
                << std::hex << type << std::dec << ", "
                << std::boolalpha << normalized << std::noboolalpha << ", "
                << stride << ", "
                << pointer
                << ")"
                << std::endl;
            DetectGLError(ss);
        }
    };

#pragma warning(push)
#pragma warning(disable: 4312)
    setVAP(0,
        VertexData::PositionElementCount,
        type, normalized, stride,
        VertexData::PositionByteOffset);
    setVAP(1,
        VertexData::ColorElementCount,
        type, normalized, stride,
        reinterpret_cast<void*>(VertexData::ColorByteOffset));
    setVAP(2,
        VertexData::TextureElementCount,
        type, normalized, stride,
        reinterpret_cast<void*>(VertexData::TextureByteOffset));
#pragma warning(pop)
}

static
void
EnableVertexAttribArray(const GLuint& index)
{
    glEnableVertexAttribArray(index);
    {
        std::stringstream ss;
        ss << "glEnableVertexAttribArray("
            << index
            << ")"
            << std::endl;
        DetectGLError(ss);
    }
}

template <typename _Ty>
void
VertexData<_Ty>::
EnableVertexAttribArray()
{
    ::EnableVertexAttribArray(0);
    ::EnableVertexAttribArray(1);
    ::EnableVertexAttribArray(2);
}

template <typename _Ty>
void
VertexData<_Ty>::
DisableVertexAttribArray()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    SuppressGLError();
}
