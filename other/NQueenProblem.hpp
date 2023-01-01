#pragma once

#include <vector>

class NQueenProblem
{
public:
    using Grid = std::vector<std::vector<int>>;

    static auto solve(int queens) -> Grid;

private:
    static constexpr int QUEEN = 1;
    static constexpr int FREE = 0;

    const int queens_;
    Grid grid_;

    NQueenProblem(int queens);

    bool solveForColumn(int col = 0);
    bool hasConflicts(int row, int col) const;
};
