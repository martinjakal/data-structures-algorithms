#include "SudokuBacktracker.hpp"

auto SudokuBacktracker::operator()() -> Grid
{
    solve();
    return grid_;
}

bool SudokuBacktracker::solve()
{
    int row;
    int col;

    if (!findEmptyPlace(row, col))
        return true;

    for (int number = 1; number <= 9; ++number)
    {
        if (noConflicts(row, col, number))
        {
            grid_[row][col] = number;

            if (solve())
                return true;

            grid_[row][col] = 0;
        }
    }

    return false;
}

bool SudokuBacktracker::findEmptyPlace(int& row, int& col) const
{
    for (row = 0; row < 9; ++row)
    {
        for (col = 0; col < 9; ++col)
        {
            if (grid_[row][col] == 0)
                return true;
        }
    }

    return false;
}

bool SudokuBacktracker::noConflicts(int row, int col, int number) const
{
    return !usedInRow(row, number) && !usedInCol(col, number)
        && !usedInBox(row - row % 3, col - col % 3, number);
}

bool SudokuBacktracker::usedInRow(int row, int number) const
{
    for (int col = 0; col < 9; ++col)
    {
        if (grid_[row][col] == number)
            return true;
    }

    return false;
}

bool SudokuBacktracker::usedInCol(int col, int number) const
{
    for (int row = 0; row < 9; ++row)
    {
        if (grid_[row][col] == number)
            return true;
    }

    return false;
}

bool SudokuBacktracker::usedInBox(int boxRow, int boxCol, int number) const
{
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            if (grid_[row + boxRow][col + boxCol] == number)
                return true;
        }
    }

    return false;
}
