#include "BigInteger.hpp"

#include <algorithm>
#include <sstream>

BigInteger::BigInteger(long long number)
{
    if (number == 0)
    {
        digits_.push_back(0);
    }
    else
    {
        if (number < 0)
        {
            sign_ = false;
            number *= -1;
        }

        while (number > 0)
        {
            digits_.push_back(number % BASE);
            number /= BASE;
        }
    } 
}

BigInteger::BigInteger(const std::string& number)
{
    int startIdx = number[0] == '-';

    if (number.empty() || number.begin() + startIdx == number.end() ||
        std::any_of(number.begin() + startIdx, number.end(), [](char c) { return !std::isdigit(c); }))
        throw std::runtime_error("Invalid string input");
    
    for (auto it = number.rbegin(); it != number.rend() - startIdx; ++it)
    {
        digits_.push_back(*it - '0');
    }
    
    sign_ = startIdx == 0;
    removeZeros();
}

BigInteger::BigInteger(const BigInteger& other) :
    digits_(other.digits_),
    sign_(other.sign_)
{}

BigInteger::BigInteger(BigInteger&& other) noexcept :
    digits_(std::move(other.digits_)),
    sign_(other.sign_)
{
    other.digits_.push_back(0);
    other.sign_ = true;
}

BigInteger& BigInteger::operator=(const BigInteger& other)
{
    digits_ = other.digits_;
    sign_ = other.sign_;
    return *this;
}

BigInteger& BigInteger::operator=(BigInteger&& other) noexcept
{
    digits_ = std::move(other.digits_);
    sign_ = other.sign_;

    other.digits_.push_back(0);
    other.sign_ = true;

    return *this;
}

void swap(BigInteger& lhs, BigInteger& rhs) noexcept
{
    using std::swap;
    swap(lhs.digits_, rhs.digits_);
    swap(lhs.sign_, rhs.sign_);
}

auto operator<<(std::ostream& os, const BigInteger& bigInteger) -> std::ostream&
{
    if (bigInteger.sign_ == false)
        os << '-';

    for (auto it = bigInteger.digits_.rbegin(); it != bigInteger.digits_.rend(); ++it)
        os << *it;
    
    return os;
}

auto BigInteger::print() const -> std::string
{
    std::stringstream out;
    out << *this;
    return out.str();
}

bool BigInteger::compare(long long number) const
{
    std::size_t numDigits = number != 0 ? static_cast<std::size_t>(std::floor(std::log10(std::abs(number)))) + 1 : 1;
    if (sign_ != number >= 0 || digitCnt() != numDigits)
        return false;

    for (std::size_t i = 0; i < digitCnt(); ++i, number /= BASE)
    {
        if (digits_[i] != std::abs(number % BASE))
            return false;
    }

    return true;
}

bool operator==(const BigInteger& lhs, const BigInteger& rhs)
{
    return lhs.sign_ == rhs.sign_ && lhs.digits_ == rhs.digits_;
}

bool operator!=(const BigInteger& lhs, const BigInteger& rhs)
{
    return !(lhs == rhs);
}

bool operator<(const BigInteger& lhs, const BigInteger& rhs)
{
    if (lhs.sign_ != rhs.sign_)
        return lhs.sign_ < rhs.sign_;

    if (lhs.digitCnt() != rhs.digitCnt())
        return lhs.sign_ == true? lhs.digitCnt() < rhs.digitCnt() : lhs.digitCnt() > rhs.digitCnt();

    for (std::size_t i = lhs.digitCnt(); i-- > 0; )
    {
        if (lhs.digits_[i] != rhs.digits_[i])
            return lhs.sign_ == true? lhs.digits_[i] < rhs.digits_[i] : lhs.digits_[i] > rhs.digits_[i];
    }

    return false;
}

bool operator>(const BigInteger& lhs, const BigInteger& rhs)
{
    return rhs < lhs;
}

bool operator<=(const BigInteger& lhs, const BigInteger& rhs)
{
    return !(lhs > rhs);
}

bool operator>=(const BigInteger& lhs, const BigInteger& rhs)
{
    return !(lhs < rhs);
}

auto BigInteger::operator++() -> BigInteger&
{
    if (this->compare(-1)) // special case -1 -> 0
    {
        --digits_[0];    
        changeSign();
    }
    else if (sign_ == true)
    {
        ++digits_[0];
        handleCarry();
    }
    else
    {
        --digits_[0];
        handleBorrow();
        removeZeros();
    }

    return *this;
}

auto BigInteger::operator++(int) -> BigInteger
{
    BigInteger result(*this);
    ++*this;
    return result;
}

auto BigInteger::operator--() -> BigInteger&
{
    if (isZero()) // special case 0 -> -1
    {
        ++digits_[0];
        changeSign();
    }
    else if (sign_ == true)
    {
        --digits_[0];
        handleBorrow();
        removeZeros();
    }
    else
    {
        ++digits_[0];
        handleCarry();
    }

    return *this;
}

auto BigInteger::operator--(int) -> BigInteger
{
    BigInteger result(*this);
    --*this;
    return result;
}

auto BigInteger::operator+(const BigInteger& other) const -> BigInteger
{
    BigInteger result(*this);
    result += other;
    return result;
}

auto BigInteger::operator+=(BigInteger other) -> BigInteger&
{
    if (other.isZero())
        return *this;

    if (sign_ != other.sign_)
    {
        other.changeSign();
        *this -= other;
    }    
    else
    {
        if (digitCnt() < other.digitCnt())
        {
            digits_.resize(other.digitCnt());
        } 

        for (std::size_t i = 0; i < other.digitCnt(); ++i)
        {
            digits_[i] += other.digits_[i];
        }

        handleCarry();
    }

    return *this;
}

auto BigInteger::operator-(const BigInteger& other) const -> BigInteger
{
    BigInteger result(*this);
    result -= other;
    return result;
}

auto BigInteger::operator-=(BigInteger other) -> BigInteger&
{
    if (other.isZero())
        return *this;

    if (*this == other)
        return *this = 0;

    if (sign_ != other.sign_)
    {
        other.changeSign();
        *this += other;
    }
    else
    {
        bool needSignChange = false;

        if (*this < other && sign_ == true || *this > other && sign_ == false)
        {
            swap(*this, other);
            needSignChange = true;
        }

        for (std::size_t i = 0; i < other.digitCnt(); ++i)
        {
            digits_[i] -= other.digits_[i];
        }

        handleBorrow();
        removeZeros();

        if (needSignChange)
            changeSign();
    }
    
    return *this;
}

auto BigInteger::operator*(const BigInteger& other) const -> BigInteger
{
    BigInteger result(*this);
    result *= other;
    return result;
}

auto BigInteger::operator*=(BigInteger other) -> BigInteger&
{
    if (this->isZero())
        return *this;

    if (other.isZero())
        return *this = 0;

    BigInteger result;
    result.digits_.resize(digitCnt() + other.digitCnt());
    result.sign_ = sign_ == other.sign_;

    for (std::size_t i = 0, p = 0; i < other.digitCnt(); ++i, ++p) // long multiplication
    {
        for (std::size_t j = 0; j < digitCnt(); ++j)
        {
            result.digits_[j + p] += digits_[j] * other.digits_[i]; // counter p adjusts position        
        }
    }

    result.handleCarry();
    result.removeZeros();

    return *this = result;
}

auto BigInteger::operator/(const BigInteger& other) const -> BigInteger
{
    BigInteger result(*this);
    result /= other;
    return result;
}

auto BigInteger::operator/=(BigInteger other) -> BigInteger&
{
    if (other.isZero())
        throw std::runtime_error("Cannot divide or mod by zero");
    
    if (this->isZero())
        return *this;

    bool needSignChange = sign_ != other.sign_;
    sign_ = true;
    other.sign_ = true;

    if (*this < other)
        return *this = 0;
    
    BigInteger result;
    BigInteger left;

    for (std::size_t i = digitCnt(); i > 0;) // long division
    {
        while (left < other && i > 0)
        {
            left = left * BASE + digits_[--i];
            result *= BASE;
        }

        while (left >= other) // division by repeated subtraction
        {
            left -= other;
            ++result;
        }
    } 
    
    if (needSignChange)
        result.changeSign();

    return *this = result;
}

auto BigInteger::operator%(const BigInteger& other) const -> BigInteger
{
    BigInteger result(*this);
    result %= other;
    return result;
}

auto BigInteger::operator%=(BigInteger other) -> BigInteger&
{
    if (other.isZero())
        throw std::runtime_error("Cannot divide or mod by zero");

    if (*this < other && sign_ == true || *this > other && sign_ == false)
        return *this;

    if (*this == other)
        return *this = 0;

    BigInteger result(*this);
    BigInteger div(*this);
    div /= other;
    div *= other;
    result -= div;

    return *this = result;
}

auto BigInteger::operator^(const BigInteger& other) const -> BigInteger
{
    BigInteger result(*this);
    result ^= other;
    return result;
}

auto BigInteger::operator^=(BigInteger other) -> BigInteger&
{
    if (this->isZero() && (other.isZero() || other.sign_ == false))
        throw std::runtime_error("Cannot divide or mod by zero");

    if (this->isZero())
        return *this;

    if (other.isZero())
        return *this = 1;

    if (other.sign_ == false) // supports whole numbers only
        return *this = 0;

    BigInteger result(1);
    BigInteger base(*this);

    while (!other.isZero()) // exponentiation by squaring
    {
        if (other.isEven())
        {
            base *= base;
            other /= 2;
        }
        else
        {
            result *= base;
            --other;
        }
    }

    return *this = result;
}

auto BigInteger::digitCnt() const -> std::size_t
{
    return digits_.size();
}

auto BigInteger::digitSum() const -> std::size_t
{
    std::size_t sum = 0;
    for (auto d : digits_)
        sum += d;
    return sum;
}

bool BigInteger::isZero() const
{
    return digits_[0] == 0 && digitCnt() == 1;
}

bool BigInteger::isEven() const
{
    return digits_[0] % 2 == 0;
}

auto BigInteger::reverse() const -> BigInteger
{
    BigInteger result(*this);
    std::reverse(result.digits_.begin(), result.digits_.end());
    result.removeZeros();
    return result;
}

bool BigInteger::isPalindrome() const
{
    for (std::size_t i = 0, j = digitCnt() - 1; i < j; ++i, --j)
    {
        if (digits_[i] != digits_[j])
            return false;
    }

    return true;
}

void BigInteger::changeSign()
{
    sign_ = !sign_;
}

// If the digit is greater or equal to base, adjust it and increase the next digit.
void BigInteger::handleCarry()
{
    for (std::size_t i = 0; i < digitCnt(); ++i)
    {
        if (digits_[i] < BASE)
            continue;

        if (i == digitCnt() - 1)
        {
            digits_.push_back(0);
        } 

        digits_[i + 1] += digits_[i] / BASE;
        digits_[i] %= BASE;
    }
}

// If the digit is negative, adjust it and decrease the next digit.
void BigInteger::handleBorrow()
{
    for (std::size_t i = 0; i < digitCnt(); ++i)
    {
        if (digits_[i] >= 0)
            continue;

        --digits_[i + 1];
        digits_[i] += BASE;
    }
}

// Remove starting zeros i.e. 003 -> 3 (this can happen in substraction and multiplication).
void BigInteger::removeZeros()
{
    while (digits_.back() == 0 && digitCnt() > 1)
    {
        digits_.pop_back();
    }

    if (isZero())
        sign_ = true;
}
