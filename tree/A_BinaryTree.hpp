#pragma once

#include <sstream>
#include <string>

#include "A_Tree.hpp"

template <typename T, typename Node>
class A_BinaryTree : public A_Tree<T, Node>
{
public:
    using Tree = A_BinaryTree<T, Node>;

    virtual ~A_BinaryTree() = default;

    bool operator==(const Tree& other) const;
    bool operator!=(const Tree& other) const;

    auto min() const -> const T&;
    auto max() const -> const T&;
    bool contains(const T& key) const;
    auto print() const -> std::string;

protected:
    virtual auto search(Node* node, const T& key) const -> Node* = 0;
    virtual auto findMin(Node* node) const -> Node* = 0;
    virtual auto findMax(Node* node) const -> Node* = 0;

    void cleanUpSubtree(Node* node);
    auto cloneSubtree(Node* parent, Node* node, Node* sentinel) -> Node*;
    bool isSameSubtree(Node* node, Node* otherNode, Node* otherSentinel) const;
    void printSubtree(Node* node, std::stringstream& out, std::string prefix, std::string childprefix) const;
};

template <typename T, typename Node>
bool A_BinaryTree<T, Node>::operator==(const Tree& other) const
{
    return isSameSubtree(this->root_, other.root_, other.nil_);
}

template <typename T, typename Node>
bool A_BinaryTree<T, Node>::operator!=(const Tree& other) const
{
    return !(*this == other);
}

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

// Recursively copy another tree into this one (starting from root). 
// The arguments are: parent - parent of the new node, node - the node in other tree, sentinel - empty leaf in other tree
// Example of function call to copy the entire tree: this->root_ = this->cloneSubtree(this->nil_, other.root_, other.nil_)
template <typename T, typename Node>
auto A_BinaryTree<T, Node>::cloneSubtree(Node* parent, Node* node, Node* sentinel) -> Node*
{
    if (node == sentinel)
        return this->nil_;

    auto copy = new Node(node->getKey());

    copy->setParent(parent);
    copy->setLeft(cloneSubtree(copy, node->getLeft(), sentinel));
    copy->setRight(cloneSubtree(copy, node->getRight(), sentinel));

    return copy;
}

template <typename T, typename Node>
bool A_BinaryTree<T, Node>::isSameSubtree(Node* node, Node* otherNode, Node* otherSentinel) const
{
    if (node == this->nil_ && otherNode == otherSentinel)
        return true;

    if (node == this->nil_ || otherNode == otherSentinel || *node != *otherNode)
        return false;

    return isSameSubtree(node->getLeft(), otherNode->getLeft(), otherSentinel) &&
        isSameSubtree(node->getRight(), otherNode->getRight(), otherSentinel);
}

// Print subtree based on Linux "tree" command.
template <typename T, typename Node>
void A_BinaryTree<T, Node>::printSubtree(Node* node, std::stringstream& out, std::string prefix, std::string childprefix) const
{
    if (node == this->nil_)
        return;

    out << prefix << node->print() << '\n';

    // Use different prefixes if a node is not the last.
    if (node->getRight() != this->nil_)
        printSubtree(node->getLeft(), out, childprefix + "├── ", childprefix + "│   ");
    else
        printSubtree(node->getLeft(), out, childprefix + "└── ", childprefix + "    ");

    printSubtree(node->getRight(), out, childprefix + "└── ", childprefix + "    ");
}