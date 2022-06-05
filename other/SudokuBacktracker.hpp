#pragma once

#include <vector>

class SudokuBacktracker
{
public:
    using Grid = std::vector<std::vector<int>>;

    SudokuBacktracker(Grid grid) : grid_(grid) {}

    auto operator()() -> Grid;

private:
    Grid grid_;

    bool solve();
    bool findEmptyPlace(int& row, int& col) const;
    bool noConflicts(int row, int col, int number) const;
    bool usedInRow(int row, int number) const;
    bool usedInCol(int col, int number) const;
    bool usedInBox(int boxRow, int boxCol, int number) const;
};

/*
Test example
std::vector<std::vector<int>> grid = {
    { 0, 0, 3, 0, 2, 0, 6, 0, 0 },
    { 9, 0, 0, 3, 0, 5, 0, 0, 1 },
    { 0, 0, 1, 8, 0, 6, 4, 0, 0 },
    { 0, 0, 8, 1, 0, 2, 9, 0, 0 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 8 },
    { 0, 0, 6, 7, 0, 8, 2, 0, 0 },
    { 0, 0, 2, 6, 0, 9, 5, 0, 0 },
    { 8, 0, 0, 2, 0, 3, 0, 0, 9 },
    { 0, 0, 5, 0, 1, 0, 3, 0, 0 } };
*/
