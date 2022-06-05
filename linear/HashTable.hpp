#pragma once

#include <list>
#include <string>
#include <vector>

class HashTable
{
public:
    HashTable(int slots);
    HashTable() = delete;

    bool contains(int value) const;
    void insert(int value);
    void remove(int value);
    auto print() const -> std::string;

private:
    const int slots_;
    std::vector<std::list<int>> table_;

    int getHash(int key) const;
};
