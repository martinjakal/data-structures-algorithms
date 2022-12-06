#pragma once

#include <sstream>
#include <string>

#include "A_Tree.hpp"

template <typename T, typename Node>
class A_BinaryTree : public A_Tree<T, Node>
{
public:
    virtual ~A_BinaryTree() = default;

    auto min() const -> const T&;
    auto max() const -> const T&;
    bool contains(const T& key) const;
    auto print() const -> std::string;

protected:
    void cleanUpSubtree(Node* node);
    void printSubtree(Node* node, std::stringstream& out, std::string prefix, std::string childprefix) const;

    virtual auto search(Node* node, const T& key) const -> Node* = 0;
    virtual auto findMin(Node* node) const -> Node* = 0;
    virtual auto findMax(Node* node) const -> Node* = 0;
};

template <typename T, typename Node>
auto A_BinaryTree<T, Node>::min() const -> const T&
{
    if (this->empty())
        throw std::runtime_error("Empty tree");

    return findMin(this->root_)->getKey();
}

template <typename T, typename Node>
auto A_BinaryTree<T, Node>::max() const -> const T&
{
    if (this->empty())
        throw std::runtime_error("Empty tree");

    return findMax(this->root_)->getKey();
}

template <typename T, typename Node>
bool A_BinaryTree<T, Node>::contains(const T& key) const
{
    return search(this->root_, key) != this->nil_;
}

template <typename T, typename Node>
auto A_BinaryTree<T, Node>::print() const -> std::string
{
    std::stringstream out;
    printSubtree(this->root_, out, "└── ", "    ");
    return out.str();
}

// Print subtree based on Linux "tree" command.
template <typename T, typename Node>
void A_BinaryTree<T, Node>::printSubtree(Node* node, std::stringstream& out, std::string prefix, std::string childprefix) const
{
    if (node == this->nil_)
        return;

    out << prefix << node->getKey() << '\n';

    // Use different prefixes if a node is not the last.
    if (node->getRight() != this->nil_)
        printSubtree(node->getLeft(), out, childprefix + "├── ", childprefix + "│   ");
    else
        printSubtree(node->getLeft(), out, childprefix + "└── ", childprefix + "    ");

    printSubtree(node->getRight(), out, childprefix + "└── ", childprefix + "    ");
}

// Deallocate entire subtree. Supposed to be called from root node in destructor.
template <typename T, typename Node>
void A_BinaryTree<T, Node>::cleanUpSubtree(Node* node)
{
    if (node == this->nil_)
        return;

    cleanUpSubtree(node->getLeft());
    cleanUpSubtree(node->getRight());
    delete node;
}
