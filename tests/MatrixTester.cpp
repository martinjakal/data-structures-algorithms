#include <cassert>
#include <iostream>

#include "../math/Matrix.hpp"

class MatrixTester
{
public:
    void fullTest() const
    {
        testMultiplication();
        testTranspose();
        testDeterminant();
        testInverse();
        testLinSolve();

        std::cout << "Passed all tests" << std::endl;
    }

    void testMultiplication() const
    {
        Matrix a = { { 1, 2, 3 }, { 4, 5, 6 } };
        Matrix b = { { 7, 8 }, { 9, 10 }, { 11, 12 } };
        Matrix result = { { 58, 64 }, { 139, 154 } };

        assert(a * b == result && "Multiplication error");
        std::cout << "Passed multiplication" << std::endl;
    }

    void testTranspose() const
    {
        Matrix m = { { 6, 4, 24 }, { 1, -9, 8 } };
        Matrix result = { { 6, 1 }, { 4, -9 }, { 24, 8 } };

        assert(m.transpose() == result && "Transpose error");
        std::cout << "Passed transpose" << std::endl;
    }

    void testDeterminant() const
    {
        Matrix m1 = { { 4, 6 }, { 3, 8 } };
        double d1 = 14;
        assert(m1.determinant() == d1 && "Determinant 2x2 error");

        Matrix m2 = { { 6, 1, 1 }, { 4, -2, 5 }, { 2, 8, 7 } };
        double d2 = -306;
        assert(m2.determinant() == d2 && "Determinant 3x3 error");

        std::cout << "Passed determinant" << std::endl;
    }

    void testInverse() const
    {
        Matrix m = { { 1, 2 }, { 3, 9 } };
        Matrix result = { { 3, -2.0 / 3 }, { -1, 1.0 / 3 } };

        assert(m.inverse() == result && "Inverse error");
        std::cout << "Passed inverse" << std::endl;
    }

    void testLinSolve() const
    {
        Matrix lhs = { { 1, 2 }, { 3, 9 } };
        Matrix rhs = { { 5 }, { 21 } };
        Matrix result = { { 1 }, { 2 } };

        assert(Matrix::linSolve(lhs, rhs) == result && "Linear solve error");
        std::cout << "Passed linear solve" << std::endl;
    }
};

int main()
{
    MatrixTester().fullTest();

    return 0;
}
