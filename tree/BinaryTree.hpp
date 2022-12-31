#pragma once

#include <queue>

#include "../node/BinaryNode.hpp"
#include "A_BinaryTree.hpp"

template <typename T>
class BinaryTree : public A_BinaryTree<T, BinaryNode<T>>
{
public:
    using Node = BinaryNode<T>;

    BinaryTree() = default;
    BinaryTree(const BinaryTree<T>& other);
    BinaryTree(BinaryTree<T>&& other) noexcept;
    auto& operator=(const BinaryTree<T>& other);
    auto& operator=(BinaryTree<T>&& other) noexcept;
    ~BinaryTree();

    friend void swap(BinaryTree<T>& lhs, BinaryTree<T>& rhs) noexcept;

    void insert(const T& key);
    void remove(const T& key);

private:
    auto search(Node* node, const T& key) const -> Node*;
    auto findMin(Node* node) const -> Node*;
    auto findMax(Node* node) const -> Node*;
};

template <typename T>
BinaryTree<T>::BinaryTree(const BinaryTree<T>& other)
{
    this->root_ = this->cloneSubtree(this->nil_, other.root_, other.nil_);
}

template <typename T>
BinaryTree<T>::BinaryTree(BinaryTree<T>&& other) noexcept
{
    swap(*this, other);
}

template <typename T>
auto& BinaryTree<T>::operator=(const BinaryTree<T>& other)
{
    auto copy(other);
    swap(*this, other);
    return *this;
}

template <typename T>
auto& BinaryTree<T>::operator=(BinaryTree<T>&& other) noexcept
{
    swap(*this, other);
    return *this;
}

template <typename T>
BinaryTree<T>::~BinaryTree()
{
    this->cleanUpSubtree(this->root_);
}

template <typename T>
void swap(BinaryTree<T>& lhs, BinaryTree<T>& rhs) noexcept
{
    using std::swap;
    swap(lhs.root_, rhs.root_);
    swap(lhs.nil_, rhs.nil_);
}

// Insert a node to the first empty leaf found.
template <typename T>
void BinaryTree<T>::insert(const T& key)
{
    auto newNode = new Node(key);

    if (this->empty())
    {
        this->root_ = newNode;
        newNode->setParent(this->nil_);
        return;
    }

    std::queue<Node*> nodes;
    nodes.push(this->root_);

    while (!nodes.empty())
    {
        auto curNode = nodes.front();
        nodes.pop();

        if (curNode->getLeft() == this->nil_)
        {
            curNode->setLeft(newNode);
            newNode->setParent(curNode);
            return;
        }
        else
            nodes.push(curNode->getLeft());

        if (curNode->getRight() == this->nil_)
        {
            curNode->setRight(newNode);
            newNode->setParent(curNode);
            return;
        }
        else
            nodes.push(curNode->getRight());
    }
}

template <typename T>
void BinaryTree<T>::remove(const T& key)
{
    if (this->empty())
        return;

    auto delNode = this->nil_;
    auto lastNode = this->nil_;

    std::queue<Node*> nodes;
    nodes.push(this->root_);

    // Traverse the tree to locate a node to be deleted and the last leaf node.
    while (!nodes.empty())
    {
        auto curNode = nodes.front();
        nodes.pop();
        lastNode = curNode;

        if (delNode == this->nil_ && curNode->getKey() == key)
            delNode = curNode;

        if (curNode->getLeft() != this->nil_)
            nodes.push(curNode->getLeft());
        if (curNode->getRight() != this->nil_)
            nodes.push(curNode->getRight());
    }

    if (delNode == this->nil_)
        return;

    // Copy the key from the last node into the node to be deleted, then delete the last node.
    if (delNode != lastNode)
        delNode->setKey(lastNode->getKey());

    auto parent = lastNode->getParent();
    if (parent == this->nil_)
        this->root_ = this->nil_;
    else
        parent->getLeft() == lastNode ? parent->setLeft(this->nil_) : parent->setRight(this->nil_);

    delete lastNode;
}

// Implemented as breadth first search.
template <typename T>
auto BinaryTree<T>::search(Node* node, const T& key) const -> Node*
{
    if (node == this->nil_)
        return node;

    std::queue<Node*> nodes;
    nodes.push(node);

    while (!nodes.empty())
    {
        auto curNode = nodes.front();
        nodes.pop();

        if (curNode->getKey() == key)
            return curNode;

        if (curNode->getLeft() != this->nil_)
            nodes.push(curNode->getLeft());
        if (curNode->getRight() != this->nil_)
            nodes.push(curNode->getRight());
    }

    return this->nil_;
}

template <typename T>
auto BinaryTree<T>::findMin(Node* node) const -> Node*
{
    if (node == this->nil_)
        return node;

    auto minNode = node;
    auto leftMinNode = findMin(node->getLeft());
    auto rightMinNode = findMin(node->getRight());

    if (leftMinNode != this->nil_ && *leftMinNode < *minNode)
        minNode = leftMinNode;
    if (rightMinNode != this->nil_ && *rightMinNode < *minNode)
        minNode = rightMinNode;

    return minNode;
}

template <typename T>
auto BinaryTree<T>::findMax(Node* node) const -> Node*
{
    if (node == this->nil_)
        return node;

    auto maxNode = node;
    auto leftMaxNode = findMax(node->getLeft());
    auto rightMaxNode = findMax(node->getRight());

    if (leftMaxNode != this->nil_ && *maxNode < *leftMaxNode)
        maxNode = leftMaxNode;
    if (rightMaxNode != this->nil_ && *maxNode < *rightMaxNode)
        maxNode = rightMaxNode;

    return maxNode;
}
