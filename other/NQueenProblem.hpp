#pragma once

#include <vector>

class NQueenProblem
{
public:
    using Grid = std::vector<std::vector<int>>;

    NQueenProblem(int queens);

    auto operator()() -> Grid;

private:
    const int queens_;
    Grid grid_;

    bool solveForColumn(int col = 0);
    bool noConflicts(int row, int col) const;
};
