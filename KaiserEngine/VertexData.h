#pragma once

#include "stdafx.h"

#include <initializer_list>
#include <sstream>
#include <gl/GL.h>

#include "loadgl.h"

template <typename _Ty>
struct VertexData
{
    struct      // positions
    {
        _Ty x;
        _Ty y;
        _Ty z;
        _Ty w;
    };
    struct      // colors
    {
        _Ty r;
        _Ty g;
        _Ty b;
        _Ty a;
    };
    struct      // texture coordinates
    {
        _Ty u;
        _Ty v;
    };

    using DataType = _Ty;

    VertexData();
    VertexData(const std::initializer_list<_Ty>& initializerList);
    ~VertexData() {}

    const static int PositionElementCount = 4;      /* x, y, z, w */
    const static int ColorElementCount = 4;         /* r, g, b, a */
    const static int TextureElementCount = 2;       /* u, v */

    static void SetVertexAttribPointer();
    static void EnableVertexAttribArray();
    static void DisableVertexAttribArray();
};

template <typename _Ty>
VertexData<_Ty>::
VertexData()
    : x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 }
    , r{ 0 }, g{ 0 }, b{ 0 }, a{ 0 }
    , u{ 0 }, v{ 0 }
{
}

template<>
VertexData<float>::
VertexData()
    : x{ 0.0f }, y{ 0.0f }, z{ 0.0f }, w{ 0.0f }
    , r{ 0.0f }, g{ 0.0f }, b{ 0.0f }, a{ 0.0f }
    , u{ 0.0f }, v{ 0.0f }
{
}

template <typename _Ty>
VertexData<_Ty>::
VertexData(const std::initializer_list<_Ty>& initializerList)
    : VertexData()
{
    _Ty* dataBegin = &(this->x);
    std::copy(initializerList.begin(), initializerList.end(), dataBegin);
}

template <typename _Ty>
void
VertexData<_Ty>::
SetVertexAttribPointer()
{
    throw std::runtime_error("Unsupported <typename _Ty>!");
}

static
void
SetVertexAttribPointer(
    const GLuint& index,
    const GLint& size,
    const GLenum& type,
    const GLboolean& normalized,
    const GLsizei& stride,
    const void* pointer)
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
}

template <>
void
VertexData<float>::
SetVertexAttribPointer()
{
    const GLenum type{ GL_FLOAT };
    const GLboolean normalized{ GL_FALSE };
    const GLsizei stride{ sizeof(VertexData<float>) };

    const static int PositionByteCount = VertexData::PositionElementCount * sizeof(VertexData::DataType);
    const static int ColorByteCount = VertexData::ColorElementCount * sizeof(VertexData::DataType);
    const static int TextureByteCount = VertexData::TextureElementCount * sizeof(VertexData::DataType);

    const static int PositionByteOffset = 0;
    const static int ColorByteOffset = PositionByteOffset + PositionByteCount;
    const static int TextureByteOffset = ColorByteOffset + ColorByteCount;

#pragma warning(push)
#pragma warning(disable: 4312)
    ::SetVertexAttribPointer(0,
        VertexData<float>::PositionElementCount,
        type, normalized, stride,
        PositionByteOffset);
    ::SetVertexAttribPointer(1,
        VertexData<float>::ColorElementCount,
        type, normalized, stride,
        reinterpret_cast<void*>(ColorByteOffset));
    ::SetVertexAttribPointer(2,
        VertexData<float>::TextureElementCount,
        type, normalized, stride,
        reinterpret_cast<void*>(TextureByteOffset));
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
