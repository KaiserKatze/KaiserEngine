#include "stdafx.h"
#include "Canvas.h"
#include "Matrix.h"

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
MakeScalingMatrix(const _Ty& Sx, const _Ty& Sy, const _Ty& Sz)
{
    MatrixQ<_Ty, 4> result;
    result[0][0] = Sx;
    result[1][1] = Sy;
    result[2][2] = Sz;
    result[3][3] = 1;
    return result;
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeRotationMatrix_x(const _Ty& Rx)
{
    MatrixQ<_Ty, 4> Mx{ IdentityMatrix() };

    Mx[1][1] = Mx[2][2] = std::cos(Rx);
    Mx[1][2] = std::sin(Rx);
    Mx[2][1] = -std::sin(Rx);

    return Mx;
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

