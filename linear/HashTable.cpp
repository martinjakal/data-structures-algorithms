#include "HashTable.hpp"

#include <sstream>

HashTable::HashTable(int slots) : slots_(slots)
{
    if (slots <= 0)
        throw std::runtime_error("Invalid number of slots");

    table_.resize(slots);
}

int HashTable::getHash(int value) const
{
    return std::abs(value % slots_);
}

bool HashTable::contains(int value) const
{
    auto hash = getHash(value);
    return std::find(table_[hash].begin(), table_[hash].end(), value) != table_[hash].end();
}

void HashTable::insert(int value)
{
    auto hash = getHash(value);
    table_[hash].push_back(value); // resolve collisions by chaining
}

void HashTable::remove(int value)
{
    auto hash = getHash(value);
    auto pos = std::find(table_[hash].begin(), table_[hash].end(), value);

    if (pos != table_[hash].end())
        table_[hash].erase(pos);
}

auto HashTable::print() const -> std::string
{
    std::stringstream out;

    for (int i = 0; i < slots_; ++i)
    {
        out << i << ": ";
        for (auto item : table_[i])
            out << item << (item != table_[i].back() ? " " : "");
        out << "\n";
    }

    return out.str();
}
