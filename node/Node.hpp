#pragma once

#include <sstream>
#include <string>

template <typename T>
class Node
{
public:
    Node(const T& key) : key_(key) {}

    auto getKey() const -> const T& { return key_; }
    void setKey(const T& key) { key_ = key; }

    bool operator==(const Node& other) const { return getKey() == other.getKey(); }
    bool operator!=(const Node& other) const { return !(*this == other); }
    bool operator< (const Node& other) const { return getKey() < other.getKey(); }

    auto print() const -> std::string;

protected:
    T key_;

    Node() = default;
};

template <typename T>
auto Node<T>::print() const -> std::string
{
    std::stringstream out;
    out << getKey();
    return out.str();
}
