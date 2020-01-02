#include "stdafx.h"
#include "Canvas.h"
#include "Matrix.h"
#include "loadgl.h"

using namespace Matrix;
using mat4 = MatrixQ<double, 4>;

Canvas::
Canvas(const MainWindow& window)
    : parent{ window }
{
}

Canvas::
~Canvas()
{
    this->dispose();
}

// @see: https://www.scratchapixel.com/
// @see: http://www.songho.ca/opengl/gl_projectionmatrix.html
template <typename _Ty>
const MatrixQ<_Ty, 4>
MakePerspectiveProjectionMatrix(
    const _Ty& aspectRatio,
    const _Ty& fieldOfView,
    const _Ty& zFar,
    const _Ty& zNear)
{
    const _Ty yScale = 1 / std::tan(fieldOfView / 2);
    const _Ty xScale = yScale / aspectRatio;
    const _Ty negatvieFrustumLength = zNear - zFar;
    MatrixQ<_Ty, 4> mq4;
    mq4.setData(0, 0, xScale);
    mq4.setData(1, 1, yScale);
    mq4.setData(2, 2, (zFar + zNear) / negatvieFrustumLength);
    mq4.setData(2, 3, (2 * zFar * zNear) / negatvieFrustumLength);
    mq4.setData(3, 2, -1);

    return mq4;
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakePerspectiveProjectionMatrix(
    const _Ty& width, const _Ty& height,
    const _Ty& fieldOfView,
    const _Ty& zFar,
    const _Ty& zNear)
{
    const _Ty aspectRatio = width / height;
    return MakePerspectiveProjectionMatrix<_Ty>(aspectRatio, fieldOfView, zFar, zNear);
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeOrthographicProjectionMatrix(
    const _Ty& right,
    const _Ty& left,
    const _Ty& top,
    const _Ty& bottom,
    const _Ty& zFar,
    const _Ty& zNear)
{
    return MatrixQ<_Ty, 4>{
        2 / (right - left), 0, 0, 0,
        0, 2 / (top - bottom), 0, 0,
        0, 0, -2 / (zFar - zNear), 0,
        (right + left) / (left - right), (top + bottom) / (bottom - top), (zFar + zNear) / (zNear - zFar), 1,
    };
}

// simplified because:
// (1) right + left = 0
// (2) top + bottom = 0
template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeOrthographicProjectionMatrix(
    const _Ty& right,
    const _Ty& top,
    const _Ty& zFar,
    const _Ty& zNear)
{
    return MakeOrthographicProjectionMatrix(right, -right, top, -top, zFar, zNear);
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeViewMatrix()
{
    return IdentityMatrix<_Ty, 4>();
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeModelMatrix()
{
    return IdentityMatrix<_Ty, 4>();
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeTranslationMatrix(const _Ty& x, const _Ty& y, const _Ty& z)
{
    return MatrixQ<_Ty, 4>{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1,
    };
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeScalingMatrix(const _Ty& Sx, const _Ty& Sy, const _Ty& Sz)
{
    return MatrixQ<_Ty, 4>{
        Sx, 0, 0, 0,
        0, Sy, 0, 0,
        0, 0, Sz, 0,
        0, 0, 0, 1,
    };
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeRotationMatrix_x(const _Ty& Rx)
{
    const _Ty cos = std::cos(Rx);
    const _Ty sin = std::sin(Rx);
    return MatrixQ<_Ty, 4>{
        1, 0, 0, 0,
        0, cos, -sin, 0,
        0, sin, cos, 0,
        0, 0, 0, 1,
    };
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeRotationMatrix_y(const _Ty& Ry)
{
    const _Ty cos = std::cos(Ry);
    const _Ty sin = std::sin(Ry);
    return MatrixQ<_Ty, 4>{
        cos, 0, sin, 0,
        0, 1, 0, 0,
        -sin, 0, cos, 0,
        0, 0, 0, 1,
    };
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeRotationMatrix_z(const _Ty& Rz)
{
    const _Ty cos = std::cos(Rz);
    const _Ty sin = std::sin(Rz);
    return MatrixQ<_Ty, 4>{
        cos, -sin, 0, 0,
        sin, cos, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
}


static
void
CALLBACK
MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::stringstream ss;
    ss << std::hex << "GL CALLBACK: "
        << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
        << " type = 0x" << type
        << ", severity = 0x" << severity
        << ", message = " << message
        << std::endl;
    OutputDebugStringA(ss.str().c_str());
}

