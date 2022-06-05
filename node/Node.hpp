#pragma once

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

protected:
    T key_;

    Node() = default;
};
