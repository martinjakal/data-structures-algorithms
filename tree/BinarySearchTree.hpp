#pragma once

#include "../node/BinaryNode.hpp"
#include "A_BinarySearchTree.hpp"

template <typename T>
class BinarySearchTree : public A_BinarySearchTree<T, BinaryNode<T>>
{
public:
    using Node = BinaryNode<T>;

    BinarySearchTree() = default;
    BinarySearchTree(const BinarySearchTree<T>& other);
    BinarySearchTree(BinarySearchTree<T>&& other) noexcept;
    auto& operator=(const BinarySearchTree<T>& other);
    auto& operator=(BinarySearchTree<T>&& other) noexcept;
    ~BinarySearchTree();

    friend void swap(BinarySearchTree<T>& lhs, BinarySearchTree<T>& rhs) noexcept;

    void insert(const T& key);
    void remove(const T& key);
};

template <typename T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree<T>& other)
{
    this->root_ = this->cloneSubtree(this->nil_, other.root_, other.nil_);
}

template <typename T>
BinarySearchTree<T>::BinarySearchTree(BinarySearchTree<T>&& other) noexcept
{
    swap(*this, other);
}

template <typename T>
auto& BinarySearchTree<T>::operator=(const BinarySearchTree<T>& other)
{
    auto copy(other);
    swap(*this, other);
    return *this;
}

template <typename T>
auto& BinarySearchTree<T>::operator=(BinarySearchTree<T>&& other) noexcept
{
    swap(*this, other);
    return *this;
}

template <typename T>
BinarySearchTree<T>::~BinarySearchTree()
{
    this->cleanUpSubtree(this->root_);
}

template <typename T>
void swap(BinarySearchTree<T>& lhs, BinarySearchTree<T>& rhs) noexcept
{
    using std::swap;
    swap(lhs.root_, rhs.root_);
    swap(lhs.nil_, rhs.nil_);
}

template <typename T>
void BinarySearchTree<T>::insert(const T& key)
{
    auto newNode = new Node(key);
    auto parent = this->nil_;
    auto child = this->root_;

    while (child != this->nil_)
    {
        parent = child;
        child = *newNode < *child ? child->getLeft() : child->getRight();
    }

    newNode->setParent(parent);

    if (parent == this->nil_)
        this->root_ = newNode;
    else
        *newNode < *parent ? parent->setLeft(newNode) : parent->setRight(newNode);
}

// Case 1: Node is leaf node - delete and pass nil to parent
// Case 2: Node has one child - delete and pass child to parent
// Case 3: Node has two children - replace by min from right subtree and delete the min
template <typename T>
void BinarySearchTree<T>::remove(const T& key)
{
    auto delNode = this->search(this->root_, key);
    if (delNode == this->nil_)
        return;

    if (delNode->getLeft() == this->nil_) // case 1 or case 2 with right child
    {
        this->transplant(delNode, delNode->getRight());
    }
    else if (delNode->getRight() == this->nil_) // case 1 or case 2 with left child
    {
        this->transplant(delNode, delNode->getLeft());
    }
    else // case 3
    {
        auto successor = this->findMin(delNode->getRight());

        if (successor->getParent() != delNode)
        {
            this->transplant(successor, successor->getRight());
            successor->setRight(delNode->getRight());
            successor->getRight()->setParent(successor);
        }

        this->transplant(delNode, successor);
        successor->setLeft(delNode->getLeft());
        successor->getLeft()->setParent(successor);
    }

    delete delNode;
}
