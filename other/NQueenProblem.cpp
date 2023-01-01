#include "NQueenProblem.hpp"

NQueenProblem::NQueenProblem(int queens): queens_(queens)
{
    grid_.resize(queens);
    for (auto& row : grid_)
        row.resize(queens, 0);
}

auto NQueenProblem::operator()() -> Grid
{
    solveForColumn();
    return grid_;
}

bool NQueenProblem::solveForColumn(int col)
{
    if (col == queens_)
        return true;

    for (int i = 0; i < queens_; i++)
    {
        if (!noConflicts(i, col))
            continue;

        grid_[i][col] = 1;

        if (solveForColumn(col + 1))
            return true;

        grid_[i][col] = 0;
    }

    return false;
}

bool NQueenProblem::noConflicts(int row, int col) const
{
    for (int i = 0; i < col; i++) // row
    {
        if (grid_[row][i] == 1)
            return false;
    }

    for (int i = row, j = col; i >= 0 && j >= 0; --i, --j) // upper diagonal
    {
        if (grid_[i][j] == 1)
            return false;
    }

    for (int i = row, j = col; i < queens_ && j >= 0; ++i, --j) // lower diagonal
    {
        if (grid_[i][j] == 1)
            return false;
    }

    return true;
}
