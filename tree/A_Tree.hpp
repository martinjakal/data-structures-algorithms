#pragma once

#include <string>

template <typename T, typename Node>
class A_Tree
{
public:
    virtual ~A_Tree() = default;

    bool empty() const;

    virtual auto min() const -> const T& = 0;
    virtual auto max() const -> const T& = 0;
    virtual bool contains(const T& key) const = 0;
    virtual void insert(const T& key) = 0;
    virtual void remove(const T& key) = 0;
    virtual auto print() const -> std::string = 0;

protected:
    Node* root_ = nullptr;
    Node* nil_ = nullptr; // empty leaf
};

template <typename T, typename Node>
bool A_Tree<T, Node>::empty() const
{
    return root_ == nil_;
}
