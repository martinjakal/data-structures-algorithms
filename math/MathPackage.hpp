#pragma once

#include <vector>

namespace math
{
namespace impl
{
template <typename T>
void generateCombinations(const T& data, std::size_t length, std::size_t startIdx, T& result, std::vector<T>& carrier, bool repeat);

} // namespace impl

// General
int digitCount(int number);
int digitSum(int number);
int countDigits(int number);
int factorial(int number);
double expBySquaring(double base, int exponent);
double median(std::vector<int> data);

// Combinatorics
template <typename T>
auto generateCombinations(const T& data, int length, bool repeat) -> std::vector<T>;

// Divisibility
auto getProperDivisors(int number) -> std::vector<int>;
auto getDivisors(int number) -> std::vector<int>;
int sumProperDivisors(int number);
bool isAbundant(int number);
bool isPerfect(int number);
bool isPairAmicable(int number1, int number2);

// Primes
bool isPrime(int number);
bool isPrime(int number, const std::vector<int>& primes);
auto getPrimeFactors(int number) -> std::vector<int>;
int countDistinctFactors(int number);
auto sieveOfEratosthenes(int limit) -> std::vector<int>;
bool isCircularPrime(int number);
bool isLeftTruncatablePrime(int number);
bool isRightTruncatablePrime(int number);
bool isTruncatablePrime(int number);

// Properties
bool isTriangular(unsigned long long number);
bool isPentagonal(unsigned long long number);
bool isPalindrome(int number, int base = 10);
bool isPermutation(int number1, int number2);
bool isPandigital(int number, int maxDigit = 9);

// Helper
int concat(int number1, int number2);

} // namespace math

namespace math
{
namespace impl
{
template <typename T>
void generateCombinations(const T& data, std::size_t length, std::size_t startIdx, T& result, std::vector<T>& carrier, bool repeat)
{
    if (length == 0)
    {
        carrier.push_back(result);
        return;
    }

    std::size_t first = repeat ? 0 : startIdx;
    for (auto i = first; i < data.size(); ++i)
    {
        result[result.size() - length] = data[i];
        generateCombinations(data, length - 1, i + 1, result, carrier, repeat);
    }
}

} // namespace impl

template <typename T>
auto generateCombinations(const T& data, int length, bool repeat) -> std::vector<T>
{
    std::vector<T> combinations;
    T result(length, ' ');
    impl::generateCombinations(data, length, 0, result, combinations, repeat);
    return combinations;
}

} // namespace math