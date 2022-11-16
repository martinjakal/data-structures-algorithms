#include <cassert>
#include <iostream>

#include "../math/Matrix.hpp"

class MatrixTester
{
public:
    void testInverse() const
    {
        Matrix m(2, 2);
        m[0][0] = 1; m[0][1] = 2;
        m[1][0] = 3; m[1][1] = 9;

        Matrix mi(2, 2);
        mi[0][0] = 3;  mi[0][1] = -2.0 / 3;
        mi[1][0] = -1; mi[1][1] = 1.0 / 3;

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
