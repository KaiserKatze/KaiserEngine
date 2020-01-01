#include "stdafx.h"
#include "Canvas.h"
#include "Matrix.h"
#include "loadgl.h"

template <typename _Ty, int N>
using MatrixQ = Matrix::MatrixQ<_Ty, N>;

Canvas::
Canvas(const MainWindow& window)
    : parent{ window }
{
}

Canvas::
~Canvas()
{
}

const RECT
Canvas::
GetClientArea() const
{
    RECT rect = { 0 };
    GetClientRect(parent.getWindowHandle(), &rect);
    return rect;
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

