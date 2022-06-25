#pragma once

#include <string>

class Matrix
{
public:
    Matrix() : Matrix(SIZE / 2, SIZE / 2) {}
    Matrix(std::size_t rowCnt, std::size_t colCnt, double element = 0);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;
    ~Matrix();

    friend void swap(Matrix& lhs, Matrix& rhs) noexcept;
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;
    auto operator[](std::size_t i) -> double*;
    auto operator[](std::size_t i) const -> const double*;
    auto print(bool debug = false) const -> std::string;

    // Scalar operations
    auto operator+=(double number) -> Matrix&;
    auto operator-=(double number) -> Matrix&;
    auto operator*=(double number) -> Matrix&;
    auto operator/=(double number) -> Matrix&;

    // Matrix operations
    auto operator+(const Matrix& other) const -> Matrix;
    auto operator-(const Matrix& other) const -> Matrix;
    auto operator*(const Matrix& other) const -> Matrix;

    // Row operations
    auto getRowCnt() const -> std::size_t { return rowCnt_; }
    bool isRowZero(std::size_t i) const;
    void swapRow(std::size_t i1, std::size_t i2);
    void appendRow(double element = 0);
    void removeRow(std::size_t iRem);
    void addToRow(std::size_t i, double number);
    void substractFromRow(std::size_t i, double number);
    void multiplyRow(std::size_t i, double number);
    void divideRow(std::size_t i, double number);
    void addRowToRow(std::size_t i1, std::size_t i2, double multiplier = 1);
    void substractRowFromRow(std::size_t i1, std::size_t i2, double multiplier = 1);
    void multiplyRowByRow(std::size_t i1, std::size_t i2, double multiplier = 1);
    void divideRowByRow(std::size_t i1, std::size_t i2, double multiplier = 1);

    // Column operations
    auto getColCnt() const -> std::size_t { return colCnt_; }
    bool isColZero(std::size_t j) const;
    void swapCol(std::size_t j1, std::size_t j2);
    void appendCol(double element = 0);
    void removeCol(std::size_t jRem);
    void addToCol(std::size_t j, double number);
    void substractFromCol(std::size_t j, double number);
    void multiplyCol(std::size_t j, double number);
    void divideCol(std::size_t j, double number);
    void addColToCol(std::size_t j1, std::size_t j2, double multiplier = 1);
    void substractColFromCol(std::size_t j1, std::size_t j2, double multiplier = 1);
    void multiplyColByCol(std::size_t j1, std::size_t j2, double multiplier = 1);
    void divideColByCol(std::size_t j1, std::size_t j2, double multiplier = 1);

    // Properties
    bool isSameDim(const Matrix& other) const;
    bool isZero() const;
    bool isSquare() const;
    bool isDiagonal() const;
    bool isIdentity() const;
    bool isSymmetrical() const;
    bool isLowerTriangular() const;
    bool isUpperTriangular() const;
    bool isTriangular() const;
    bool isSingular() const;
    bool isSparse() const;
    bool isRowEchelonForm() const;
    bool isReducedRowEchelonForm() const;

    // Manipulation
    void makeIdentity();
    void autofill(double element = 0);
    auto submatrix(std::size_t i, std::size_t j) const -> Matrix;
    auto transpose() const -> Matrix;
    double trace() const;
    double determinant() const;
    double sparsity() const;
    auto inverse() const -> Matrix;
    auto rowEchelonForm() const -> Matrix;
    auto reducedRowEchelonForm() const -> Matrix;
    auto rank() const -> std::size_t;
    static auto linSolve(const Matrix& a, const Matrix& b) -> Matrix;

private:
    static constexpr double EPSILON = 0.000001;
    static constexpr int SIZE = 10;

    double** data_ = nullptr;

    std::size_t rowCnt_ = 0;
    std::size_t colCnt_ = 0;
    std::size_t rowDim_ = 0;
    std::size_t colDim_ = 0;

    bool isSameElement(double a, double b) const;
    auto updateSize(std::size_t s) const -> std::size_t;
    void alloc();
    void resize();
    void cleanup();
};
