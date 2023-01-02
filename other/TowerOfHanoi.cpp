#include "TowerOfHanoi.hpp"

#include <iostream>

TowerOfHanoi::TowerOfHanoi(int discs) : discs_(discs)
{
    for (int i = discs_; i > 0; --i)
        left_.push_back(i);
}

void TowerOfHanoi::solve(int discs)
{
    auto toh = TowerOfHanoi(discs);
    toh.print();
    toh.solve(toh.discs_, toh.left_, toh.right_, toh.middle_);
}

void TowerOfHanoi::solve(int discs, Rod& start, Rod& end, Rod& help)
{
    if (discs == 1)
    {
        move(discs, start, end);
        return;
    }

    solve(discs - 1, start, help, end);
    move(discs, start, end);
    solve(discs - 1, help, end, start);
}

void TowerOfHanoi::move(int disc, Rod& start, Rod& end)
{
    start.pop_back();
    end.push_back(disc);
    print();
}

void TowerOfHanoi::print() const
{
    std::cout << "L";
    for (auto disc : left_)
        std::cout << " " << disc;
    std::cout << "\nM";
    for (auto disc : middle_)
        std::cout << " " << disc;
    std::cout << "\nR";
    for (auto disc : right_)
        std::cout << " " << disc;
    std::cout << "\n\n";
}
