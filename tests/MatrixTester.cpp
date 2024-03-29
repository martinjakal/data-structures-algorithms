#include <cassert>
#include <iostream>

#include "../math/Matrix.hpp"

class MatrixTester
{
public:
    void fullTest() const
    {
        additionTest();
        subtractionTest();
        multiplicationTest();
        transposeTest();
        determinantTest();
        inverseTest();
        rowEchelonFormTest();
        reducedRowEchelonFormTest();
        linSolveTest();

        std::cout << "Passed all tests" << std::endl;
    }

    void additionTest() const
    {
        Matrix a = { { 4, 8 }, { 3, 7 } };
        Matrix b = { { 1, 0 }, { 5, 2 } };
        Matrix expected = { { 5, 8 }, { 8, 9 } };

        auto result = a + b;

        assert(result == expected && "Addition error");
        std::cout << "Passed addition" << std::endl;
    }

    void subtractionTest() const
    {
        Matrix a = { { 2, 8 }, { 0, 9 } };
        Matrix b = { { 5, 6 }, { 11, 3 } };
        Matrix expected = { { -3, 2 }, { -11, 6 } };

        auto result = a - b;

        assert(result == expected && "Subtraction error");
        std::cout << "Passed subtraction" << std::endl;
    }

    void multiplicationTest() const
    {
        Matrix a = { { 1, 2, 3 }, { 4, 5, 6 } };
        Matrix b = { { 7, 8 }, { 9, 10 }, { 11, 12 } };
        Matrix expected = { { 58, 64 }, { 139, 154 } };

        auto result = a * b;

        assert(result == expected && "Multiplication error");
        std::cout << "Passed multiplication" << std::endl;
    }

    void transposeTest() const
    {
        Matrix m = { { 6, 4, 24 }, { 1, -9, 8 } };
        Matrix expected = { { 6, 1 }, { 4, -9 }, { 24, 8 } };

        auto result = m.transpose();

        assert(result == expected && "Transpose error");
        std::cout << "Passed transpose" << std::endl;
    }

    void determinantTest() const
    {
        Matrix m1 = { { 4, 6 }, { 3, 8 } };
        double d1 = 14;
        assert(m1.determinant() == d1 && "Determinant 2x2 error");

        Matrix m2 = { { 6, 1, 1 }, { 4, -2, 5 }, { 2, 8, 7 } };
        double d2 = -306;
        assert(m2.determinant() == d2 && "Determinant 3x3 error");

        std::cout << "Passed determinant" << std::endl;
    }

    void inverseTest() const
    {
        Matrix m = { { 1, 2 }, { 3, 9 } };
        Matrix expected = { { 3, -2.0 / 3 }, { -1, 1.0 / 3 } };

        auto result = m.inverse();

        assert(result == expected && "Inverse error");
        std::cout << "Passed inverse" << std::endl;
    }

    void rowEchelonFormTest() const
    {
        Matrix m = { { 1, 1, 2 }, { 1, 2, 3 }, { 3, 4, 5 } };
        Matrix expected = { { 1, 1, 2 }, { 0, 1, 1 }, { 0, 0, -2 } };

        auto result = m.rowEchelonForm();

        assert(result.isRowEchelonForm() && "Row echelon form (check) error");
        assert(result == expected && "Row echelon form (calc) error");
        std::cout << "Passed row echelon form" << std::endl;
    }

    void reducedRowEchelonFormTest() const
    {
        Matrix m = { { 1, 2, 5 }, { 2, -1, 5 } };
        Matrix expected = { { 1, 0, 3 }, { 0, 1, 1 } };

        auto result = m.reducedRowEchelonForm();

        assert(result.isReducedRowEchelonForm() && "Reduced row echelon form (check) error");
        assert(result == expected && "Reduced row echelon form (calc) error");
        std::cout << "Passed reduced row echelon form" << std::endl;
    }

    void linSolveTest() const
    {
        Matrix lhs = { { 1, 2 }, { 3, 9 } };
        Matrix rhs = { { 5 }, { 21 } };
        Matrix expected = { { 1 }, { 2 } };

        auto result = Matrix::linSolve(lhs, rhs);

        assert(result == expected && "Linear solve error");
        std::cout << "Passed linear solve" << std::endl;
    }
};

int main()
{
    MatrixTester().fullTest();

    return 0;
}
