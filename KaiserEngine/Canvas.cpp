#include "stdafx.h"
#include "Canvas.h"
#include "Matrix.h"

template <typename _Ty, int N>
using MatrixQ = Matrix::MatrixQ<_Ty, N>;

Canvas::
Canvas()
{
}

Canvas::
~Canvas()
{
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
    MatrixQ<_Ty, 4> My{ IdentityMatrix() };

    My[0][0] = My[2][2] = std::cos(Ry);
    My[2][0] = std::sin(Ry);
    My[0][2] = -std::sin(Ry);

    return My;
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeRotationMatrix_z(const _Ty& Rz)
{
    MatrixQ<_Ty, 4> Mz{ IdentityMatrix() };

    Mz[0][0] = Mz[1][1] = std::cos(Rz);
    Mz[0][1] = std::sin(Rz);
    Mz[1][0] = -std::sin(Rz);

    return Mz;
}

