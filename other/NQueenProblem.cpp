#include "NQueenProblem.hpp"

constexpr int NQueenProblem::FREE;

NQueenProblem::NQueenProblem(int queens) : queens_(queens)
{
    grid_.resize(queens_);
    for (auto& row : grid_)
        row.resize(queens_, FREE);
}

auto NQueenProblem::solve(int queens) -> Grid
{
    auto nqp = NQueenProblem(queens);
    nqp.placeQueenInColumn();
    return nqp.grid_;
}

bool NQueenProblem::placeQueenInColumn(int col)
{
    if (col == queens_)
        return true;

    for (int i = 0; i < queens_; ++i)
    {
        if (hasConflicts(i, col))
            continue;

        grid_[i][col] = QUEEN;

        if (placeQueenInColumn(col + 1))
            return true;

        grid_[i][col] = FREE;
    }

    return false;
}

bool NQueenProblem::hasConflicts(int row, int col) const
{
    for (int i = 0; i < col; ++i) // row
    {
        if (grid_[row][i] == QUEEN)
            return true;
    }

    for (int i = row, j = col; i >= 0 && j >= 0; --i, --j) // upper diagonal
    {
        if (grid_[i][j] == QUEEN)
            return true;
    }

    for (int i = row, j = col; i < queens_ && j >= 0; ++i, --j) // lower diagonal
    {
        if (grid_[i][j] == QUEEN)
            return true;
    }

    return false;
}
