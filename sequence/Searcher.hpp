#pragma once

#include <algorithm>
#include <functional>

namespace search
{
template <typename It, typename Compare = std::less<>>
auto findMinimum(It first, It last, Compare cmp = Compare{}) -> It
{
    if (first == last)
        return last;

    auto minIt = first;
    ++first;

    for (; first != last; ++first)
    {
        if (cmp(*first, *minIt))
            minIt = first;
    }

    return minIt;
}

template <typename It, typename Compare = std::greater<>>
auto findMaximum(It first, It last, Compare cmp = Compare{}) -> It
{
    if (first == last)
        return last;

    auto maxIt = first;
    ++first;

    for (; first != last; ++first)
    {
        if (cmp(*first, *maxIt))
            maxIt = first;
    }

    return maxIt;
}

template <typename It, typename T, typename Compare = std::equal_to<>>
auto linearSearch(It first, It last, const T& value, Compare cmp = Compare{}) -> It
{
    for (; first != last; ++first)
    {
        if (cmp(*first, value))
            return first;
    }

    return last;
}

template <typename It, typename T, typename Compare = std::less<>>
auto binarySearch(It first, It last, const T& value, Compare cmp = Compare{}) -> It
{
    if (first == last)
        return last;

    auto dist = std::distance(first, last);
    auto lowIt = first;
    auto highIt = first;
    std::advance(highIt, dist - 1);

    while (dist > 0 && !cmp(value, *lowIt) && !cmp(*highIt, value))
    {
        const auto pos = dist / 2;
        auto newIt = lowIt;
        std::advance(newIt, pos);

        if (cmp(*newIt, value))
        {
            std::advance(lowIt, pos + 1);
        }
        else if (cmp(value, *newIt))
        {
            highIt = lowIt;
            std::advance(highIt, pos);
        }
        else
            return std::lower_bound(lowIt, newIt, value);

        dist = std::distance(lowIt, highIt);
    }

    return last;
}

template <typename It, typename T, typename Compare = std::less<>>
auto interpolationSearch(It first, It last, const T& value, Compare cmp = Compare{}) -> It
{
    if (first == last)
        return last;

    auto dist = std::distance(first, last);
    auto lowIt = first;
    auto highIt = first;
    std::advance(highIt, dist - 1);

    while (dist > 0 && !cmp(value, *lowIt) && !cmp(*highIt, value))
    {
        const auto pos = (value - *lowIt) * dist / (*highIt - *lowIt);
        auto newIt = lowIt;
        std::advance(newIt, pos);

        if (cmp(*newIt, value))
        {
            std::advance(lowIt, pos + 1); 
        }
        else if (cmp(value, *newIt))
        {
            highIt = lowIt;
            std::advance(highIt, pos - 1);
        }
        else
            return std::lower_bound(lowIt, newIt, value);

        dist = std::distance(lowIt, highIt);
    }

    return last;
}

} // namespace search
