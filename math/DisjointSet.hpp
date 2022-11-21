#pragma once

#include <sstream>
#include <string>
#include <unordered_map>

template <typename T>
class DisjointSet
{
public:
    void makeSet(const T& key);
    auto findSet(const T& key) const -> const T&;
    void unionSet(const T& key1, const T& key2);
    auto countSet() const -> std::size_t;
    auto print() const -> std::string;

private:
    std::unordered_map<T, std::pair<T, int>> data_; // key: set, value: <parent, size>

    void merge(const T& small, const T& big);
};

template <typename T>
void DisjointSet<T>::makeSet(const T& key)
{
    if (data_.find(key) == data_.end())
        data_[key] = { key, 1 };
}

template <typename T>
auto DisjointSet<T>::findSet(const T& key) const -> const T&
{
    const auto& parent = data_.at(key).first;
    return parent == key ? parent : findSet(parent);
}

template <typename T>
void DisjointSet<T>::unionSet(const T& key1, const T& key2)
{
    const auto& parent1 = findSet(key1);
    const auto& parent2 = findSet(key2);

    // If the elements come from distinct sets, merge smaller set into bigger set.
    if (parent1 != parent2)
        data_.at(parent1).second < data_.at(parent2).second ? merge(parent1, parent2) : merge(parent2, parent1);
}

template <typename T>
void DisjointSet<T>::merge(const T& small, const T& big)
{
    data_.at(big).second += data_.at(small).second;
    data_.at(small) = { data_.at(big).first, 0 };
}

template <typename T>
auto DisjointSet<T>::countSet() const -> std::size_t
{
    std::size_t setCnt = 0;

    for (const auto& set : data_)
    {
        if (set.second.second != 0)
            ++setCnt;
    }

    return setCnt;
}

template <typename T>
auto DisjointSet<T>::print() const -> std::string
{
    std::stringstream out;

    for (const auto& item : data_)
        out << (!out.str().empty() ? " " : "") << item.first << ":" <<
        item.second.first << "(" << item.second.second << ")";

    return out.str();
}
