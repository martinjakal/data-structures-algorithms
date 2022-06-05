#pragma once

#include <vector>

class TowerOfHanoi
{
public:
    using Rod = std::vector<int>;

    TowerOfHanoi(int discs);

    void operator()();

private:
    int discs_;
    Rod left_;
    Rod middle_;
    Rod right_;

    void solve(int discs, Rod& start, Rod& end, Rod& help);
    void move(int disc, Rod& start, Rod& end);
    void print() const;
};
