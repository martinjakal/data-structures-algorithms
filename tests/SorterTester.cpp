#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "../sequence/Sorter.hpp"

namespace sort
{
class SorterTester
{
public:
    void fullTest() const
    {
        std::vector<int> data = { 8, 2, 5, 6, 1, 4, 3, 9, 7 };

        using It = decltype(data)::iterator;
        using T = decltype(data);
        using Compare = std::less<>;

        sortTest<It, T, Compare>("bubble", bubbleSort<It, Compare>, data);
        sortTest<It, T, Compare>("insertion", insertionSort<It, Compare>, data);
        sortTest<It, T, Compare>("selection", selectionSort<It, Compare>, data);
        sortTest<It, T, Compare>("quick", quickSort<It, Compare>, data);
        sortTest<It, T, Compare>("merge", mergeSort<It, Compare>, data);
        sortTest<It, T, Compare>("heap", heapSort<It, Compare>, data);

        std::cout << "Passed all tests" << std::endl;
    }

    template <typename It, typename T, typename Compare>
    void sortTest(const std::string& type, std::function<void(It, It, Compare)> sort, T data, Compare cmp = Compare{}) const
    {
        sort(data.begin(), data.end(), cmp);
        assert(std::is_sorted(data.begin(), data.end(), cmp) && "Sort error");
        std::cout << "Passed " << type << " sort" << std::endl;
    }
};

} // namespace sort

int main()
{
    sort::SorterTester().fullTest();

    return 0;
}
