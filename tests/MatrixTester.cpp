#include <cassert>
#include <iostream>

#include "../math/Matrix.hpp"

class MatrixTester
{
public:
    void testInverse() const
    {
        Matrix m = { { 1, 2 }, { 3, 9 } };
        Matrix mi = { { 3, -2.0 / 3 }, { -1, 1.0 / 3 } };

        assert(m.inverse() == mi && "Inverse error");
    }

    void fullTest() const
    {
        testInverse();
        std::cout << "Passed all tests" << std::endl;
    }
};

int main()
{
    MatrixTester().fullTest();

    return 0;
}
