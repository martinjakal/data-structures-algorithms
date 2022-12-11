#pragma once

#include <string>

// Special member variable nil_ is used to represent an empty leaf. In many cases this would work
// just as alias for null pointer, but sometimes a special "empty" node called sentinel is created 
// and all empty leaves point to the sentinel (useful in self-balancing trees).
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
