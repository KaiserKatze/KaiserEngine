#pragma once

template <typename _Ty, typename std::enable_if<std::is_floating_point<_Ty>::value>::type* = nullptr>
const _Ty
degrees2radians(_Ty degrees)
{
    static const auto pi = std::acos(-1);
    return static_cast<_Ty>(degrees / 180 * pi);
}

namespace Matrix
{
    // the entries in this class are stored in column-major order
    // template argument 'typename _Ty' can be one of the following:
    //  - real number (such as int, float, double, etc.)
    //  - complex number (std::complex<double>, etc.)
    //  - matrix
    template <typename _Ty, int Height, int Width>
    class Matrix
    {
        static_assert(Height > 0, "Template argument 'Height' has negative value!");
        static_assert(Width > 0, "Template argument 'Width' has negative value!");
    private:
        _Ty data[Width * Height];

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
            memcpy(data, buffer, count * sizeof(_Ty));
        }

        using same_type = Matrix<_Ty, Height, Width>;

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

        void setData(int row, int column, const _Ty& value)
        {
            data[convert2index(row, column)] = value;
        }

        const _Ty& getData(int row, int column) const
        {
            return data[convert2index(row, column)];
        }

        constexpr bool isVector() const { return (Height == 1 && Width > 1) || (Width == 1 && Height > 1); }

        constexpr bool isSquare() const { return Height == Width && Height > 1; }

        const bool isDiagonal() const
        {
            if (!isSquare()) return false;
            for (int column = 0; column < Width; column++)
            {
                for (int row = 0; row < Height; row++)
                {
                    if (row == column) continue;
                    if (getData(row, column) != 0)
                        return false;
                }
            }
            return true;
        }

        constexpr int getWidth() const { return Width; }
        constexpr int getHeight() const { return Height; }

        same_type operator+(same_type other)
        {
            same_type result;

            for (int i = 0; i < Width * Height; i++)
            {
                result.data[i] = this->data[i] + other.data[i];
            }

            return result;
        }

        same_type operator-(same_type other)
        {
            same_type result;

            for (int i = 0; i < Width * Height; i++)
            {
                result.data[i] = this->data[i] - other.data[i];
            }

            return result;
        }

        same_type operator*(const _Ty& multiplier)
        {
            same_type result;

            for (int i = 0; i < Width * Height; i++)
            {
                result.data[i] = this->data[i] * multiplier;
            }

            return result;
        }

        same_type operator/(const _Ty& divider)
        {
            same_type result;

            for (int i = 0; i < Width * Height; i++)
            {
                result.data[i] = this->data[i] / divider;
            }

            return result;
        }

        void operator+=(same_type other)
        {
            for (int i = 0; i < Width * Height; i++)
            {
                this->data[i] += other.data[i];
            }
        }

        void operator-=(same_type other)
        {
            for (int i = 0; i < Width * Height; i++)
            {
                this->data[i] -= other.data[i];
            }
        }

        void operator*=(const _Ty& multiplier)
        {
            for (int i = 0; i < Width * Height; i++)
            {
                this->data[i] *= multiplier;
            }
        }

        void operator/=(const _Ty& divider)
        {
            for (int i = 0; i < Width * Height; i++)
            {
                this->data[i] /= divider;
            }
        }

        bool operator==(const same_type& other)
        {
            return this == &other
                || std::equal(std::begin(data), std::end(data),
                    std::begin(other.data), std::end(other.data));
        }

        using Transpose = Matrix<_Ty, Width, Height>;

        const Transpose transpose() const
        {
            Transpose result;

            for (int i = 0; i < Width; i++)
            {
                for (int j = 0; j < Height; j++)
                {
                    result.setData(i, j, getData(j, i));
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
                result.setData(i - RowSrc, j - ColSrc, getData(i, j));
            }
            }

            return result;
        }

        // print internal data structure, regardless of whether the matrix is transposed or not
        const std::string toString() const
        {
            std::stringstream ss;

            for (int column = 0; column < Width; column++)
            {
                ss << "| ";
                for (int row = 0; row < Height; row++)
                {
                    ss << getData(row, column)
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

    // Outter: Height, Width
    // Inner: IHeight, IWidth
    template <typename _Ty, int IHeight, int IWidth, int Height, int Width>
    class Matrix<Matrix<_Ty, IHeight, IWidth>, Height, Width>
    {
        static_assert(IHeight > 0, "Template argument 'IHeight' has negative value!");
        static_assert(IWidth > 0, "Template argument 'IWidth' has negative value!");
        static_assert(Height > 0, "Template argument 'Height' has negative value!");
        static_assert(Width > 0, "Template argument 'Width' has negative value!");

    public:
        using inner_type = Matrix<_Ty, IHeight, IWidth>;
        using same_type = Matrix<inner_type, Height, Width>;
        using equivalent_type = Matrix<_Ty, Height * IHeight, Width * IWidth>;

    private:
        inner_type data[Height * Width];

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

    public:
        Matrix()
        {
            memset(data, 0, sizeof(data));
        }

        Matrix(std::initializer_list<inner_type> init)
            : Matrix()
        {
            const int count = std::min<int>(init.size(), Height * Width);
            int idx = 0;
            auto itr = init.begin();
            while (idx < count && itr != init.end())
            {
                data[idx++] = *itr++;
            }
        }

        ~Matrix() {}

        constexpr int getHeight() const { return Height; }
        constexpr int getWidth() const { return Width; }

        void setData(int row, int column, const inner_type& value)
        {
            data[convert2index(row, column)] = value;
        }

        const inner_type& getData(int row, int column) const
        {
            return data[convert2index(row, column)];
        }

        same_type operator*(const _Ty& multiplier)
        {
            same_type result;

            for (int i = 0; i < Width * Height; i++)
            {
                result.data[i] = this->data[i] * multiplier;
            }

            return result;
        }

        same_type operator/(const _Ty& divider)
        {
            same_type result;

            for (int i = 0; i < Width * Height; i++)
            {
                result.data[i] = this->data[i] / divider;
            }

            return result;
        }

        void operator+=(same_type other)
        {
            for (int i = 0; i < Width * Height; i++)
            {
                this->data[i] += other.data[i];
            }
        }

        void operator-=(same_type other)
        {
            for (int i = 0; i < Width * Height; i++)
            {
                this->data[i] -= other.data[i];
            }
        }

        void operator*=(const _Ty& multiplier)
        {
            for (int i = 0; i < Width * Height; i++)
            {
                this->data[i] *= multiplier;
            }
        }

        void operator/=(const _Ty& divider)
        {
            for (int i = 0; i < Width * Height; i++)
            {
                this->data[i] /= divider;
            }
        }

        bool operator==(const same_type& other)
        {
            return this == &other
                || std::equal(std::begin(data), std::end(data),
                    std::begin(other.data), std::end(other.data));
        }

        using Transpose = Matrix<Matrix<_Ty, IWidth, IHeight>, Width, Height>;

        const Transpose transpose() const
        {
            Transpose result;

            for (int i = 0; i < Transpose::getWidth(); i++) // result column
            {
                for (int j = 0; j < Transpose::getHeight(); j++) // result row
                {
                    result.setData(j, i, getData(i, j).transpose());
                }
            }

            return result;
        }

        const equivalent_type merge() const
        {
            equivalent_type result;

            for (int i = 0; i < Height; i++)
                for (int j = 0; j < Width; j++)
                {
                    const inner_type& entry = getData(i, j);
                    for (int p = 0; p < entry.getHeight(); p++)
                        for (int q = 0; q < entry.getWidth(); q++)
                            result.setData(i * IHeight + p, j * IWidth + q, entry.getData(p, q));
                }

            return result;
        }

        const std::string toString() const
        {
            return merge().toString();
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

            inner_type& operator[](const int& secondIndex)
            {
                const int index = parent.convert2index(first, secondIndex);
                return const_cast<_Ty&>(parent.data[index]);
            }

            const inner_type operator[](const int& secondIndex) const
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

    // identity matrix

    template <typename _Ty, int N>
    class IdentityMatrix
        : public MatrixQ<_Ty, N>
    {
    public:
        IdentityMatrix()
            : MatrixQ<_Ty, N>()
        {
            for (int i = 0; i < N; i++)
                this->setData(i, i, 1);
        }
    };

    template <>
    class IdentityMatrix<int, 2>
        : public MatrixQ<int, 2>
    {
    public:
        IdentityMatrix()
            : MatrixQ<int, 2>({ 1, 0, 0, 1 })
        {}
    };

    template <>
    class IdentityMatrix<int, 3>
        : public MatrixQ<int, 3>
    {
    public:
        IdentityMatrix()
            : MatrixQ<int, 3>({ 1, 0, 0, 0, 1, 0, 0, 0, 1 })
        {}
    };

    template <>
    class IdentityMatrix<int, 4>
        : public MatrixQ<int, 4>
    {
    public:
        IdentityMatrix()
            : MatrixQ<int, 4>({ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 })
        {}
    };

    template <>
    class IdentityMatrix<float, 2>
        : public MatrixQ<float, 2>
    {
    public:
        IdentityMatrix()
            : MatrixQ<float, 2>({ 1.0f, 0.0f, 0.0f, 1.0f })
        {}
    };

    template <>
    class IdentityMatrix<float, 3>
        : public MatrixQ<float, 3>
    {
    public:
        IdentityMatrix()
            : MatrixQ<float, 3>({ 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f })
        {}
    };

    template <>
    class IdentityMatrix<float, 4>
        : public MatrixQ<float, 4>
    {
    public:
        IdentityMatrix()
            : MatrixQ<float, 4>({ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f })
        {}
    };

    template <>
    class IdentityMatrix<double, 2>
        : public MatrixQ<double, 2>
    {
    public:
        IdentityMatrix()
            : MatrixQ<double, 2>({ 1.0, 0.0, 0.0, 1.0 })
        {}
    };

    template <>
    class IdentityMatrix<double, 3>
        : public MatrixQ<double, 3>
    {
    public:
        IdentityMatrix()
            : MatrixQ<double, 3>({ 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 })
        {}
    };

    template <>
    class IdentityMatrix<double, 4>
        : public MatrixQ<double, 4>
    {
    public:
        IdentityMatrix()
            : MatrixQ<double, 4>({ 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 })
        {}
    };

    using Matrix2i = MatrixQ<int, 2>;
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

    // merely a minor optimization
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

    template <int N> struct is_even { static constexpr bool value = (N & 1) == 0; };

    template <typename _Ty, int N, typename std::enable_if<is_even<N>::value>::type* = nullptr>
    MatrixQ<MatrixQ<_Ty, N / 2>, 2> partition(const Matrix<_Ty, N, N>& parent)
    {
        return MatrixQ<MatrixQ<_Ty, N / 2>, 2>{
            parent.submatrix<0, N / 2, 0, N / 2>(),
            parent.submatrix<N / 2, N, 0, N / 2>(),
            parent.submatrix<0, N / 2, N / 2, N>(),
            parent.submatrix<N / 2, N, N / 2, N>(),
        };
    }

} /* NAMESPACE: Matrix */
