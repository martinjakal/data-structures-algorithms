#include "Matrix.hpp"

#include <cmath>
#include <sstream>

Matrix::Matrix(std::size_t rowCnt, std::size_t colCnt, double element) :
    rowCnt_(rowCnt),
    colCnt_(colCnt),
    rowDim_(updateSize(rowCnt_)),
    colDim_(updateSize(colCnt_))
{
    alloc();

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < colCnt_; ++j)
        {
            data_[i][j] = element;
        }
    }
}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> data) :
    rowCnt_(data.size()),
    colCnt_(data.size() > 0 ? data.begin()->size() : 0),
    rowDim_(updateSize(rowCnt_)),
    colDim_(updateSize(colCnt_))
{
    alloc();

    std::size_t i = 0;
    std::size_t j = 0;

    for (auto row : data)
    {
        for (auto element : row)
        {
            data_[i][j] = element;
            ++j;
        }

        ++i;
        j = 0;
    }
}

Matrix::Matrix(const Matrix& other) :
    rowCnt_(other.rowCnt_),
    colCnt_(other.colCnt_),
    rowDim_(other.rowDim_),
    colDim_(other.colDim_)
{
    alloc();

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < colCnt_; ++j)
        {
            data_[i][j] = other.data_[i][j];
        }
    }
}

Matrix::Matrix(Matrix&& other) noexcept
{
    swap(*this, other);
}

Matrix& Matrix::operator=(const Matrix& other)
{
    Matrix copy(other);
    swap(*this, copy);
    return *this;
}

Matrix& Matrix::operator=(Matrix&& other) noexcept
{
    swap(*this, other);
    return *this;
}

Matrix::~Matrix()
{
    cleanup();
}

void swap(Matrix& lhs, Matrix& rhs) noexcept
{
    using std::swap;
    swap(lhs.data_, rhs.data_);
    swap(lhs.rowCnt_, rhs.rowCnt_);
    swap(lhs.colCnt_, rhs.colCnt_);
    swap(lhs.rowDim_, rhs.rowDim_);
    swap(lhs.colDim_, rhs.colDim_);
}

auto Matrix::updateSize(std::size_t s) const -> std::size_t
{
    return s + SIZE - s % SIZE;
}

void Matrix::alloc()
{
    data_ = new double* [rowDim_];
    for (std::size_t i = 0; i < rowDim_; ++i)
    {
        data_[i] = new double[colDim_];
    }
}

// The matrix allocates more memory than the current row/col count to support 
// insertion/removal of the data. The size is determined as row/col count rounded up
// to the nearest multiple of SIZE variable (both dimensions can have different sizes).
// Resize is called each time after row/col is inserted/removed, size is updated 
// and matrix is resized if necessary.
void Matrix::resize()
{
    std::size_t newRowDim = updateSize(rowCnt_);
    std::size_t newColDim = updateSize(colCnt_);

    if (newRowDim == rowDim_ && newColDim == colDim_)
        return;

    double** newData = new double* [newRowDim];
    for (std::size_t i = 0; i < newRowDim; ++i)
    {
        newData[i] = new double[newColDim];
    }

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < colCnt_; ++j)
        {
            newData[i][j] = data_[i][j];
        }
    }

    cleanup();
    data_ = newData;
    rowDim_ = newRowDim;
    colDim_ = newColDim;
}

void Matrix::cleanup()
{
    for (std::size_t i = 0; i < rowDim_; ++i)
    {
        delete[] data_[i];
    }

    delete[] data_;
    data_ = nullptr;
}

bool Matrix::isSameElement(double a, double b) const
{
    return std::abs(a - b) < EPSILON;
}

bool Matrix::operator==(const Matrix& other) const
{
    if (!isSameDim(other))
        return false;

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < colCnt_; ++j)
        {
            if (!isSameElement(data_[i][j], other.data_[i][j]))
                return false;
        }
    }

    return true;
}

bool Matrix::operator!=(const Matrix& other) const
{
    return !(*this == other);
}

auto Matrix::operator[](std::size_t i) -> double*
{
    if (i >= rowCnt_)
        throw std::runtime_error("Index out of range");

    return data_[i];
}

auto Matrix::operator[](std::size_t i) const -> const double*
{
    if (i >= rowCnt_)
        throw std::runtime_error("Index out of range");

    return data_[i];
}

auto Matrix::print(bool debug) const -> std::string
{
    std::stringstream out;

    if (data_ != nullptr)
    {
        for (std::size_t i = 0; i < rowCnt_; ++i)
        {
            for (std::size_t j = 0; j < colCnt_; ++j)
                out << data_[i][j] << (j < colCnt_ - 1 ? " " : "");
            out << "\n";
        }
    }

    if (debug)
    {
        out << "rowCnt " << rowCnt_ << " colCnt " << colCnt_ << "\n";
        out << "rowDim " << rowDim_ << " colDim " << colDim_ << "\n";
    }

    return out.str();
}

auto Matrix::operator+=(double number) -> Matrix&
{
    for (std::size_t i = 0; i < rowCnt_; ++i)
        addToRow(i, number);

    return *this;
}

auto Matrix::operator-=(double number) -> Matrix&
{
    for (std::size_t i = 0; i < rowCnt_; ++i)
        substractFromRow(i, number);

    return *this;
}

auto Matrix::operator*=(double number) -> Matrix&
{
    for (std::size_t i = 0; i < rowCnt_; ++i)
        multiplyRow(i, number);

    return *this;
}

auto Matrix::operator/=(double number) -> Matrix&
{
    for (std::size_t i = 0; i < rowCnt_; ++i)
        divideRow(i, number);

    return *this;
}

auto Matrix::operator+(const Matrix& other) const -> Matrix
{
    if (!isSameDim(other))
        throw std::runtime_error("Invalid dimensions");

    Matrix result(*this);

    for (std::size_t i = 0; i < result.rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < result.colCnt_; ++j)
        {
            result.data_[i][j] += other.data_[i][j];
        }
    }

    return result;
}

auto Matrix::operator-(const Matrix& other) const -> Matrix
{
    if (!isSameDim(other))
        throw std::runtime_error("Invalid dimensions");

    Matrix result(*this);

    for (std::size_t i = 0; i < result.rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < result.colCnt_; ++j)
        {
            result.data_[i][j] -= other.data_[i][j];
        }
    }

    return result;
}

auto Matrix::operator*(const Matrix& other) const -> Matrix
{
    if (colCnt_ != other.rowCnt_)
        throw std::runtime_error("Invalid dimensions");

    Matrix result(rowCnt_, other.colCnt_);

    for (std::size_t i = 0; i < result.rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < result.colCnt_; ++j)
        {
            for (std::size_t k = 0; k < colCnt_; ++k)
            {
                result.data_[i][j] += data_[i][k] * other.data_[k][j];
            }
        }
    }

    return result;
}

bool Matrix::isRowZero(std::size_t i) const
{
    if (i >= rowCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t j = 0; j < colCnt_; ++j)
    {
        if (data_[i][j] != 0)
            return false;
    }

    return true;
}

void Matrix::swapRow(std::size_t i1, std::size_t i2)
{
    if (i1 >= rowCnt_ || i2 >= rowCnt_)
        throw std::runtime_error("Index out of range");

    std::swap(data_[i1], data_[i2]);
}

void Matrix::appendRow(double element)
{
    for (std::size_t j = 0; j < colCnt_; ++j)
    {
        data_[rowCnt_][j] = element;
    }

    ++rowCnt_;
    resize();
}

void Matrix::removeRow(std::size_t iRem)
{
    if (iRem >= rowCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t i = iRem; i < rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < colCnt_; ++j)
        {
            data_[i][j] = data_[i + 1][j];
        }
    }

    --rowCnt_;
    resize();
}

void Matrix::addToRow(std::size_t i, double number)
{
    if (i >= rowCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t j = 0; j < colCnt_; ++j)
        data_[i][j] += number;
}

void Matrix::substractFromRow(std::size_t i, double number)
{
    if (i >= rowCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t j = 0; j < colCnt_; ++j)
        data_[i][j] -= number;
}

void Matrix::multiplyRow(std::size_t i, double number)
{
    if (i >= rowCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t j = 0; j < colCnt_; ++j)
        data_[i][j] *= number;
}

void Matrix::divideRow(std::size_t i, double number)
{
    if (i >= rowCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t j = 0; j < colCnt_; ++j)
        data_[i][j] /= number;
}

void Matrix::addRowToRow(std::size_t i1, std::size_t i2, double multiplier)
{
    if (i1 >= rowCnt_ || i2 >= rowCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t j = 0; j < colCnt_; ++j)
        data_[i2][j] += multiplier * data_[i1][j];
}

void Matrix::substractRowFromRow(std::size_t i1, std::size_t i2, double multiplier)
{
    if (i1 >= rowCnt_ || i2 >= rowCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t j = 0; j < colCnt_; ++j)
        data_[i2][j] -= multiplier * data_[i1][j];
}

void Matrix::multiplyRowByRow(std::size_t i1, std::size_t i2, double multiplier)
{
    if (i1 >= rowCnt_ || i2 >= rowCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t j = 0; j < colCnt_; ++j)
        data_[i2][j] *= multiplier * data_[i1][j];
}

void Matrix::divideRowByRow(std::size_t i1, std::size_t i2, double multiplier)
{
    if (i1 >= rowCnt_ || i2 >= rowCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t j = 0; j < colCnt_; ++j)
        data_[i2][j] /= multiplier * data_[i1][j];
}

bool Matrix::isColZero(std::size_t j) const
{
    if (j >= colCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        if (data_[i][j] != 0)
            return false;
    }

    return true;
}

void Matrix::swapCol(std::size_t j1, std::size_t j2)
{
    if (j1 >= colCnt_ || j2 >= colCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t i = 0; i < rowCnt_; ++i)
        std::swap(data_[i][j1], data_[i][j2]);
}

void Matrix::appendCol(double element)
{
    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        data_[i][colCnt_] = element;
    }

    ++colCnt_;
    resize();
}

void Matrix::removeCol(std::size_t jRem)
{
    if (jRem >= colCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        for (std::size_t j = jRem; j < colCnt_; ++j)
        {
            data_[i][j] = data_[i][j + 1];
        }
    }

    --colCnt_;
    resize();
}

void Matrix::addToCol(std::size_t j, double number)
{
    if (j >= colCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t i = 0; i < rowCnt_; ++i)
        data_[i][j] += number;
}

void Matrix::substractFromCol(std::size_t j, double number)
{
    if (j >= colCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t i = 0; i < rowCnt_; ++i)
        data_[i][j] -= number;
}

void Matrix::multiplyCol(std::size_t j, double number)
{
    if (j >= colCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t i = 0; i < rowCnt_; ++i)
        data_[i][j] *= number;
}

void Matrix::divideCol(std::size_t j, double number)
{
    if (j >= colCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t i = 0; i < rowCnt_; ++i)
        data_[i][j] /= number;
}

void Matrix::addColToCol(std::size_t j1, std::size_t j2, double multiplier)
{
    if (j1 >= colCnt_ || j2 >= colCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t i = 0; i < rowCnt_; ++i)
        data_[i][j2] += multiplier * data_[i][j1];
}

void Matrix::substractColFromCol(std::size_t j1, std::size_t j2, double multiplier)
{
    if (j1 >= colCnt_ || j2 >= colCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t i = 0; i < rowCnt_; ++i)
        data_[i][j2] -= multiplier * data_[i][j1];
}

void Matrix::multiplyColByCol(std::size_t j1, std::size_t j2, double multiplier)
{
    if (j1 >= colCnt_ || j2 >= colCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t i = 0; i < rowCnt_; ++i)
        data_[i][j2] *= multiplier * data_[i][j1];
}

void Matrix::divideColByCol(std::size_t j1, std::size_t j2, double multiplier)
{
    if (j1 >= colCnt_ || j2 >= colCnt_)
        throw std::runtime_error("Index out of range");

    for (std::size_t i = 0; i < rowCnt_; ++i)
        data_[i][j2] /= multiplier * data_[i][j1];
}

bool Matrix::isSameDim(const Matrix& other) const
{
    return rowCnt_ == other.rowCnt_ && colCnt_ == other.colCnt_;
}

bool Matrix::isZero() const
{
    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        if (!isRowZero(i))
            return false;
    }
    return true;
}

bool Matrix::isSquare() const
{
    return rowCnt_ == colCnt_;
}

bool Matrix::isDiagonal() const
{
    if (!isSquare())
        return false;

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < colCnt_; ++j)
        {
            if (i != j && data_[i][j] != 0 || i == j && data_[i][j] == 0)
                return false;
        }
    }

    return true;
}

bool Matrix::isIdentity() const
{
    if (!isSquare())
        return false;

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < colCnt_; ++j)
        {
            if (i != j && data_[i][j] != 0 || i == j && data_[i][j] != 1)
                return false;
        }
    }

    return true;
}

bool Matrix::isSymmetrical() const
{
    if (!isSquare())
        return false;

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        for (std::size_t j = i + 1; j < colCnt_; ++j)
        {
            if (data_[i][j] != data_[j][i])
                return false;
        }
    }

    return true;
}

bool Matrix::isLowerTriangular() const
{
    if (!isSquare())
        return false;

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < i; ++j)
        {
            if (data_[i][j] != 0)
                return false;
        }
    }

    return true;
}

bool Matrix::isUpperTriangular() const
{
    if (!isSquare())
        return false;

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        for (std::size_t j = i + 1; j < colCnt_; ++j)
        {
            if (data_[i][j] != 0)
                return false;
        }
    }

    return true;
}

bool Matrix::isTriangular() const
{
    return isLowerTriangular() || isUpperTriangular();
}

bool Matrix::isSingular() const
{
    return isSquare() && determinant() == 0;
}

bool Matrix::isSparse() const
{
    return sparsity() > 0.5;
}

// 1. The first non-zero element (called the leading entry or pivot) in each row is 1.
// 2. Each leading entry is in a column to the right of the leading entry in the previou row.
// 3. Zero rows, if there are any, are below non-zero rows.
bool Matrix::isRowEchelonForm() const
{
    bool foundZeroRow = false;
    std::size_t baseZeros = 0;

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        if (!foundZeroRow)
        {
            std::size_t zeroCounter = 0;

            for (std::size_t j = 0; j < colCnt_; ++j) // find the pivot and count zeros to the left
            {
                if (data_[i][j] == 0)
                    ++zeroCounter;
                else
                {
                    if (data_[i][j] != 1)
                        return false;

                    break;
                }
            }

            if (i != 0) // row must have more zeros before pivot than the previous row
            {
                if (zeroCounter <= baseZeros)
                    return false;
            }

            baseZeros = zeroCounter;

            if (zeroCounter == colCnt_)
                foundZeroRow = true;
        }
        else // if zero row is found, all following rows must also be zero
        {
            if (!isRowZero(i))
                return false;
        }
    }

    return true;
}

// 1. The matrix is in row echelon form.
// 2. The leading entry in each row is the only non-zero element in its column.
bool Matrix::isReducedRowEchelonForm() const
{
    if (!isRowEchelonForm())
        return false;

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < colCnt_; ++j)
        {
            if (data_[i][j] != 0)
            {
                for (std::size_t i2 = 0; i2 < rowCnt_; ++i2)
                {
                    if (data_[i2][j] != 0 && i2 != i)
                        return false;
                }

                break;
            }
        }
    }

    return true;
}

void Matrix::makeIdentity()
{
    if (!isSquare())
        throw std::runtime_error("Non-square matrix");

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < colCnt_; ++j)
        {
            data_[i][j] = i == j;
        }
    }
}

void Matrix::autofill(double element)
{
    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < colCnt_; ++j)
        {
            data_[i][j] = element++;
        }
    }
}

auto Matrix::submatrix(std::size_t i, std::size_t j) const -> Matrix
{
    Matrix result(*this);
    result.removeRow(i);
    result.removeCol(j);
    return result;
}

auto Matrix::transpose() const -> Matrix
{
    Matrix result(colCnt_, rowCnt_);

    for (std::size_t i = 0; i < colCnt_; ++i)
    {
        for (std::size_t j = 0; j < rowCnt_; ++j)
        {
            result.data_[i][j] = data_[j][i];
        }
    }

    return result;
}

double Matrix::trace() const
{
    if (!isSquare())
        return 0;

    double trace = 0;

    for (std::size_t i = 0; i < rowCnt_; ++i)
        trace += data_[i][i];

    return trace;
}

double Matrix::determinant() const
{
    if (!isSquare())
        throw std::runtime_error("Non-square matrix");

    if (rowCnt_ == 1)
        return data_[0][0];
    else
    {
        double determinant = 0;
        int sign = 1;

        for (std::size_t j = 0; j < colCnt_; ++j)
        {
            determinant += sign * data_[0][j] * this->submatrix(0, j).determinant();
            sign *= -1;
        }

        return determinant;
    }
}

double Matrix::sparsity() const
{
    std::size_t totalElements = rowCnt_ * colCnt_;
    std::size_t zeroElements = 0;

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < colCnt_; ++j)
        {
            zeroElements += data_[i][j] == 0;
        }
    }

    return static_cast<double>(zeroElements) / totalElements;
}

auto Matrix::inverse() const -> Matrix
{
    if (isSingular())
        throw std::runtime_error("Singular matrix");

    Matrix result(*this);
    double mainDeterminant = result.determinant();

    for (std::size_t i = 0; i < rowCnt_; ++i)
    {
        for (std::size_t j = 0; j < colCnt_; ++j)
        {
            int sign = (i + j) % 2 == 0 ? 1 : -1;
            result.data_[j][i] = sign * this->submatrix(i, j).determinant() / mainDeterminant;
        }
    }

    return result;
}

auto Matrix::rowEchelonForm() const -> Matrix
{
    Matrix result(*this);
    std::size_t ordered = 0;

    for (std::size_t j = 0; j < colCnt_; ++j)
    {
        for (std::size_t i = ordered; i < rowCnt_; ++i)
        {
            if (result.data_[i][j] != 0) // locate pivot row by finding first non-zero element in a column
            {
                if (i != ordered) // move pivot row to the ordered part
                {
                    result.swapRow(i, ordered);
                }

                /* temporarily disabled
                if (result.data_[ordered][j] != 1) // set first element in the pivot row to 1
                {
                    result.multiplyRow(ordered, 1.0 / result.data_[ordered][j]);
                }
                */

                for (std::size_t i2 = ordered + 1; i2 < rowCnt_; ++i2) // set elements below first element in current pivot row to 0
                {
                    if (result.data_[i2][j] != 0)
                        result.addRowToRow(ordered, i2, -result.data_[i2][j]);
                }

                ++ordered;
                break;
            }
        }
    }

    return result;
}

auto Matrix::reducedRowEchelonForm() const -> Matrix
{
    Matrix result(isRowEchelonForm() ? *this : this->rowEchelonForm());

    for (std::size_t i = rowCnt_ - 1; i > 0; --i)
    {
        for (std::size_t j = 0; j < colCnt_; ++j)
        {
            if (result.data_[i][j] != 0)
            {
                for (std::size_t i2 = 0; i2 < i; ++i2) // set elements above first element in pivot row to 0
                {
                    if (result.data_[i2][j] != 0)
                        result.addRowToRow(i, i2, -result.data_[i2][j]);
                }

                break;
            }
        }
    }

    return result;
}

auto Matrix::rank() const -> std::size_t
{
    if (!isRowEchelonForm())
        return this->rowEchelonForm().rank();

    std::size_t nonZeroRows = 0;

    for (std::size_t i = 0; i < rowCnt_; ++i)
        nonZeroRows += !isRowZero(i);

    return nonZeroRows;
}

auto Matrix::linSolve(const Matrix& a, const Matrix& b) -> Matrix
{
    return a.inverse() * b;
}
