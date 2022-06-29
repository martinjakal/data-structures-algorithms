#pragma once

#include <algorithm>
#include <functional>

namespace sort
{
template <typename It, typename Compare = std::less<>>
void bubbleSort(It first, It last, Compare cmp = Compare{})
{
    for (auto it = first; first != last; last = it)
    {
        it = first;
        for (auto cur = first, prev = first; ++cur != last; ++prev)
        {
            if (cmp(*cur, *prev))
            {
                std::iter_swap(cur, prev);
                it = cur;
            }
        }
    }
}

template <typename It, typename Compare = std::less<>>
void insertionSort(It first, It last, Compare cmp = Compare{})
{
    for (auto it = first; it != last; ++it)
    {
        const auto insertionIt = std::upper_bound(first, it, *it, cmp);
        std::rotate(insertionIt, it, std::next(it));
    }
}

template <typename It, typename Compare = std::less<>>
void selectionSort(It first, It last, Compare cmp = Compare{})
{
    for (auto it = first; it != last; ++it)
    {
        const auto selectionIt = std::min_element(it, last, cmp);
        std::iter_swap(selectionIt, it);
    }
}

template <typename It, typename Compare = std::less<>>
void quickSort(It first, It last, Compare cmp = Compare{})
{
    const auto size = std::distance(first, last);
    if (size <= 1)
        return;

    const auto pivot = *std::next(first, size / 2);
    const auto midIt1 = std::partition(first, last, [&](const auto& element) { return cmp(element, pivot); });
    const auto midIt2 = std::partition(midIt1, last, [&](const auto& element) { return !cmp(pivot, element); });

    quickSort(first, midIt1, cmp);
    quickSort(midIt2, last, cmp);
}

template <typename It, typename Compare = std::less<>>
void mergeSort(It first, It last, Compare cmp = Compare{})
{
    const auto size = std::distance(first, last);
    if (size <= 1)
        return;

    const auto midIt = std::next(first, size / 2);
    mergeSort(first, midIt, cmp);
    mergeSort(midIt, last, cmp);
    std::inplace_merge(first, midIt, last, cmp);
}

template <typename It, typename Compare = std::less<>>
void heapSort(It first, It last, Compare cmp = Compare{})
{
    std::make_heap(first, last, cmp);
    std::sort_heap(first, last, cmp);
}

} // namespace sort
