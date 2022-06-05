#include <cassert>
#include <cmath>
#include <iostream>

#include "BigInteger.hpp"

class BigIntegerTester
{
public:
    void pairTest(long long a, long long b, bool print = false) const
    {
        const BigInteger x(a);
        const BigInteger y(b);

        if (print)
        {
            std::cout << "Values" << std::endl;
            std::cout << "x: " << x << std::endl;
            std::cout << "y: " << y << std::endl;
        }

        assert(++BigInteger(a) == a + 1 && "Increment error");
        assert(--BigInteger(b) == b - 1 && "Decrement error");

        assert(x + y == a + b && "Addition error");
        assert(x - y == a - b && "Subtraction error");
        assert(x * y == a * b && "Multiplication error");

        if (y != 0)
        {
            assert(x / y == a / b && "Division error");
            assert(x % y == a % b && "Modulo error");
        }

        if (x != 0 || y > 0)
            assert((x ^ y) == static_cast<long long>(std::pow(a, b)) && "Exponentiation error");

        if (print)
        {
            std::cout << "Arithmetics" << std::endl;
            std::cout << "x + y: " << x + y << std::endl;
            std::cout << "x - y: " << x - y << std::endl;
            std::cout << "x * y: " << x * y << std::endl;

            try
            {
                std::cout << "x / y: " << x / y << std::endl;
                std::cout << "x % y: " << x % y << std::endl;
            }
            catch (...)
            {
                std::cout << "x / y: nan" << std::endl;
                std::cout << "x % y: nan" << std::endl;
            }

            try
            {
                std::cout << "x ^ y: " << (x ^ y) << std::endl;
            }
            catch (...)
            {
                std::cout << "x ^ y: nan" << std::endl;
            }
        }

        std::cout << "Passed (" << x << ", " << y << ")" << std::endl;
    }

    void fullTest(bool print = false) const
    {
        pairTest(10, 6, print);
        pairTest(6, 10, print);
        pairTest(-6, -10, print);
        pairTest(-10, -6, print);
        pairTest(10, -6, print);
        pairTest(-6, 10, print);
        pairTest(-10, 6, print);
        pairTest(6, -10, print);
        pairTest(5, -5, print);
        pairTest(5, 0, print);
        pairTest(0, 5, print);
        pairTest(0, 0, print);

        std::cout << "Passed all tests" << std::endl;
    }
};

int main()
{
    BigIntegerTester().fullTest();

    return 0;
}
