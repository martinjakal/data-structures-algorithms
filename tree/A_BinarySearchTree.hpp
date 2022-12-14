#pragma once

#include "A_BinaryTree.hpp"

template <typename T, typename Node>
class A_BinarySearchTree : public A_BinaryTree<T, Node>
{
public:
    virtual ~A_BinarySearchTree() = default;

protected:
    auto search(Node* node, const T& key) const -> Node*;
    auto findMin(Node* node) const -> Node*;
    auto findMax(Node* node) const -> Node*;

    void transplant(Node* oldNode, Node* newNode);
};

template <typename T, typename Node>
auto A_BinarySearchTree<T, Node>::search(Node* node, const T& key) const -> Node*
{
    while (node != this->nil_ && node->getKey() != key)
        node = key < node->getKey() ? node->getLeft() : node->getRight();

    return node;
}

template <typename T, typename Node>
auto A_BinarySearchTree<T, Node>::findMin(Node* node) const -> Node*
{
    while (node->getLeft() != this->nil_)
        node = node->getLeft();

    return node;
}

template <typename T, typename Node>
auto A_BinarySearchTree<T, Node>::findMax(Node* node) const -> Node*
{
    while (node->getRight() != this->nil_)
        node = node->getRight();

    return node;
}

template <typename T, typename Node>
void A_BinarySearchTree<T, Node>::transplant(Node* oldNode, Node* newNode)
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