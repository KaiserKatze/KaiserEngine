#pragma once

template <typename _Ty, typename std::enable_if<std::is_floating_point<_Ty>::value>::type* = nullptr>
const _Ty
degrees2radians(_Ty degrees)
{
    static const auto pi = std::acos(-1);
    return static_cast<_Ty>(degrees / 180 * pi);
}


// the entries in this class are stored in column-major order
template <typename _Ty, int Height, int Width>
class Matrix
{
private:
    _Ty data[Width * Height];

    // pos = (row, column)
    int convert2index(const std::pair<int, int>& pos) const
    {
        return convert2index(pos.first, pos.second);
    }

    int convert2index(const int& row, const int& column) const
    {
        if (row < 0)
            throw std::domain_error("Invalid argument: row < 0!");
        if (row >= Height)
            throw std::domain_error("Invalid argument: row >= Height!");
        if (column < 0)
            throw std::domain_error("Invalid argument: column < 0!");
        if (column >= Width)
            throw std::domain_error("Invalid argument: column >= Width!");
        return row + column * Height;   // column-first
    }

    Matrix(const _Ty* buffer, int count)
        : Matrix()
    {
        count = std::min<int>(count, Height * Width);
        memcpy(static_cast<void*>(data), buffer, count * sizeof(_Ty));
    }

public:
    Matrix()
        : data{ 0 }
    {
    }

    Matrix(std::initializer_list<_Ty> init)
        : Matrix(std::vector<_Ty>(init.begin(), init.end()).data(), init.size())
    {
    }

    ~Matrix() {}

    constexpr bool IsVector() const
    {
        return Height == 1 && Width > 0
            || Width == 1 && Height > 0;
    }

    Matrix<_Ty, Height, Width> operator+(Matrix<_Ty, Height, Width> other)
    {
        Matrix<_Ty, Height, Width> result;

        for (int i = 0; i < Width * Height; i++)
        {
            result.data[i] = this->data[i] + other.data[i];
        }

        return result;
    }

    void operator*=(const _Ty& multiplier)
    {
        for (int i = 0; i < Width * Height; i++)
        {
            this->data[i] *= multiplier;
        }
    }

    _Ty& operator[](const std::pair<int, int>& pos)
    {
        return data[convert2index(pos)];
    }

    const Matrix<_Ty, Width, Height> transpose() const
    {
        Matrix<_Ty, Width, Height> result;
        for (int i = 0; i < Width; i++)
        {
        for (int j = 0; j < Height; j++)
        {
            result[i][j] = data[convert2index(j, i)];
        }
        }
        return result;
    }

    template <int RowSrc, int RowDst, int ColSrc, int ColDst>
    Matrix<_Ty, RowDst - RowSrc, ColDst - ColSrc> submatrix() const
    {
        using ret_type = Matrix<_Ty, RowDst - RowSrc, ColDst - ColSrc>;
        ret_type result;

        for (int i = RowSrc; i < RowDst; i++)
        {
        for (int j = ColSrc; j < ColDst; j++)
        {
            result[i - RowSrc][j - ColSrc] = data[convert2index(i, j)];
        }
        }

        return result;
    }

    // print internal data structure, regardless of whether the matrix is transposed or not
    const std::string ToString() const
    {
        std::stringstream ss;

        for (int row = 0; row < Height; row++)
        {
            ss << "| ";
            for (int column = 0; column < Width; column++)
            {
                ss << data[convert2index(row, column)]
                    << " ";
            }
            ss << "|"
                << std::endl;
        }

        return ss.str();
    }

public:

    // enables invoking `matrix[row][column]`

    class MatrixVector
    {
    private:
        const Matrix& parent;
        const int first;

    public:
        MatrixVector(const Matrix& matrix, const int& firstIndex)
            : parent{ matrix }
            , first{ firstIndex }
        {
        }

        _Ty& operator[](const int& secondIndex)
        {
            const int index = parent.convert2index(first, secondIndex);
            return const_cast<_Ty&>(parent.data[index]);
        }

        const _Ty operator[](const int& secondIndex) const
        {
            const int index = parent.convert2index(first, secondIndex);
            return parent.data[index];
        }
    };

    MatrixVector operator[](const int& firstIndex)
    {
        return MatrixVector(*this, firstIndex);
    }

    const MatrixVector operator[](const int& firstIndex) const
    {
        return MatrixVector(*this, firstIndex);
    }
};

// squre matrix
template <typename _Ty, int N>
using MatrixQ = Matrix<_Ty, N, N>;

template <typename _Ty, int N>
MatrixQ<_Ty, N> IdentityMatrix()
{
    MatrixQ<_Ty, N> result;
    for (int i = 0; i < N; i++)
    {
        result[i][i] = 1;
    }
    return result;
}

using Matrix3i = MatrixQ<int, 3>;
using Matrix4i = MatrixQ<int, 4>;
using Matrix3f = MatrixQ<float, 3>;
using Matrix4f = MatrixQ<float, 4>;
using Matrix3d = MatrixQ<double, 3>;
using Matrix4d = MatrixQ<double, 4>;

// multiplying a (M x P) matrix and a (P x N) matrix,
// result: a (M x N) matrix
// no optimization is applied in the following function
// @see: https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm
template <typename _Ty, int M, int P, int N>
Matrix<_Ty, M, N> operator*(const Matrix<_Ty, M, P>& first, const Matrix<_Ty, P, N>& second)
{
    Matrix<_Ty, M, N> result;

    for (int i = 0; i < M; i++)
    {
    for (int j = 0; j < N; j++)
    {
        _Ty sum = 0;

        for (int k = 0; k < P; k++)
        {
            _Ty a_ik = first[i][k];
            _Ty b_kj = second[k][j];
            sum += a_ik * b_kj;
        }

        result[i][j] = sum;
    }
    }

    return result;
}

template <typename _Ty>
MatrixQ<_Ty, 2> operator*(const MatrixQ<_Ty, 2>& first, const MatrixQ<_Ty, 2>& second)
{
    const _Ty a11 = first[0][0];
    const _Ty a12 = first[0][1];
    const _Ty a21 = first[1][0];
    const _Ty a22 = first[1][1];
    const _Ty b11 = second[0][0];
    const _Ty b12 = second[0][1];
    const _Ty b21 = second[1][0];
    const _Ty b22 = second[1][1];

    const _Ty c11 = a11 * b11 + a12 * b21;
    const _Ty c12 = a11 * b12 + a12 * b22;
    const _Ty c21 = a21 * b11 + a22 * b21;
    const _Ty c22 = a21 * b12 + a22 * b22;

    MatrixQ<_Ty, 2> result{ c11, c21, c12, c22 };
    return result;
}
