#pragma once

#include "../node/BinaryNode.hpp"
#include "A_BinaryTree.hpp"

template <typename T>
class BinarySearchTree: public A_BinaryTree<T, BinaryNode<T>>
{
public:
    using Node = BinaryNode<T>;

    BinarySearchTree() = default;
    ~BinarySearchTree();

    void insert(const T& key);
    void remove(const T& key);

private:
    auto search(Node* node, const T& key) const -> Node*;
    auto findMin(Node* node) const -> Node*;
    auto findMax(Node* node) const -> Node*;

    void transplant(Node* oldNode, Node* newNode);
};

template <typename T>
BinarySearchTree<T>::~BinarySearchTree()
{
    this->cleanUpSubtree(this->root_);
}

template <typename T>
void BinarySearchTree<T>::insert(const T& key)
{
    auto newNode = new Node(key);
    auto prevNode = this->nil_;
    auto curNode = this->root_;

    while (curNode != this->nil_)
    {
        prevNode = curNode;
        curNode = *newNode < *curNode ? curNode->getLeft() : curNode->getRight();
    }

    newNode->setParent(prevNode);

    if (prevNode == this->nil_)
        this->root_ = newNode;
    else
        *newNode < *prevNode ? prevNode->setLeft(newNode) : prevNode->setRight(newNode);
}

// Case 1: Node is leaf node - delete and pass nil to parent
// Case 2: Node has one child - delete and pass child to parent
// Case 3: Node has two children - replace by min from right subtree and delete the min
template <typename T>
void BinarySearchTree<T>::remove(const T& key)
{
    auto delNode = search(this->root_, key);
    if (delNode == this->nil_)
        return;

    if (delNode->getLeft() == this->nil_) // case 1 or case 2 with right child
    {
        transplant(delNode, delNode->getRight());
    }
    else if (delNode->getRight() == this->nil_) // case 1 or case 2 with left child
    {
        transplant(delNode, delNode->getLeft());
    }
    else // case 3
    {
        auto successor = findMin(delNode->getRight());

        if (successor->getParent() != delNode)
        {
            transplant(successor, successor->getRight());
            successor->setRight(delNode->getRight());
            successor->getRight()->setParent(successor);
        }

        transplant(delNode, successor);
        successor->setLeft(delNode->getLeft());
        successor->getLeft()->setParent(successor);
    }

    delete delNode;
}

template <typename T>
void BinarySearchTree<T>::transplant(Node* oldNode, Node* newNode)
{
    auto oldParent = oldNode->getParent();

    if (oldParent == this->nil_)
        this->root_ = newNode;
    else if (oldNode == oldParent->getLeft())
        oldParent->setLeft(newNode);
    else
        oldParent->setRight(newNode);

    if (newNode != this->nil_)
        newNode->setParent(oldParent);
}

template <typename T>
auto BinarySearchTree<T>::search(Node* node, const T& key) const -> Node*
{
    while (node != this->nil_ && node->getKey() != key)
        node = key < node->getKey() ? node->getLeft() : node->getRight();

    return node;
}

template <typename T>
auto BinarySearchTree<T>::findMin(Node* node) const -> Node*
{
    while (node->getLeft() != this->nil_)
        node = node->getLeft();

    return node;
}

template <typename T>
auto BinarySearchTree<T>::findMax(Node* node) const -> Node*
{
    while (node->getRight() != this->nil_)
        node = node->getRight();

    return node;
}
