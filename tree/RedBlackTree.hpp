#pragma once

#include "../node/ColorType.hpp"
#include "../node/RedBlackNode.hpp"
#include "A_BinarySearchTree.hpp"

// A red black tree is a binary tree that satisfies the following properties:
// 1. Every node is either red or black.
// 2. The root is black.
// 3. Every leaf (nil) is black.
// 4. If a node is red, then both its children are black.
// 5. For each node, all simple paths from the node to descendant leaves contain the same number of black nodes.
template <typename T>
class RedBlackTree : public A_BinarySearchTree<T, RedBlackNode<T>>
{
public:
    using Node = RedBlackNode<T>;

    RedBlackTree();
    RedBlackTree(const RedBlackTree<T>& other);
    RedBlackTree(RedBlackTree<T>&& other) noexcept;
    auto& operator=(const RedBlackTree<T>& other);
    auto& operator=(RedBlackTree<T>&& other) noexcept;
    ~RedBlackTree();

    friend void swap(RedBlackTree<T>& lhs, RedBlackTree<T>& rhs) noexcept;

    void insert(const T& key);
    void remove(const T& key);

private:
    void initSentinel();
    void cloneTree(Node* otherRoot, Node* otherSentinel);
    void cloneColorsSubtree(Node* node, Node* otherNode, Node* otherSentinel);

    void leftRotate(Node* node);
    void rightRotate(Node* node);
    void insertFixup(Node* node);
    void removeFixup(Node* node);
};

// A sentinel (black node with no parent and no children) is used to represent leaf nodes 
// to avoid excessive null pointer checking when handling corner cases.
template <typename T>
void RedBlackTree<T>::initSentinel()
{
    auto sentinel = new Node;
    this->nil_ = sentinel;
};

template <typename T>
RedBlackTree<T>::RedBlackTree()
{
    initSentinel();
    this->root_ = this->nil_;
};

template <typename T>
RedBlackTree<T>::RedBlackTree(const RedBlackTree<T>& other)
{
    initSentinel();
    cloneTree(other.root_, other.nil_);
}

template <typename T>
RedBlackTree<T>::RedBlackTree(RedBlackTree<T>&& other) noexcept
{
    swap(*this, other);
}

template <typename T>
auto& RedBlackTree<T>::operator=(const RedBlackTree<T>& other)
{
    auto copy(other);
    swap(*this, other);
    return *this;
}

template <typename T>
auto& RedBlackTree<T>::operator=(RedBlackTree<T>&& other) noexcept
{
    swap(*this, other);
    return *this;
}

template <typename T>
RedBlackTree<T>::~RedBlackTree()
{
    this->cleanUpSubtree(this->root_);
    delete this->nil_;
}

template <typename T>
void swap(RedBlackTree<T>& lhs, RedBlackTree<T>& rhs) noexcept
{
    using std::swap;
    swap(lhs.root_, rhs.root_);
    swap(lhs.nil_, rhs.nil_);
}

template <typename T>
void RedBlackTree<T>::cloneTree(Node* otherRoot, Node* otherSentinel)
{
    this->root_ = this->cloneSubtree(this->nil_, otherRoot, otherSentinel);
    cloneColorsSubtree(this->root_, otherRoot, otherSentinel);
}

template <typename T>
void RedBlackTree<T>::cloneColorsSubtree(Node* node, Node* otherNode, Node* otherSentinel)
{
    if (otherNode == otherSentinel)
        return;

    node->recolor(otherNode);
    cloneColorsSubtree(node->getLeft(), otherNode->getLeft(), otherSentinel);
    cloneColorsSubtree(node->getRight(), otherNode->getRight(), otherSentinel);
}

template <typename T>
void RedBlackTree<T>::insert(const T& key)
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
    newNode->setLeft(this->nil_);
    newNode->setRight(this->nil_);

    if (parent == this->nil_)
        this->root_ = newNode;
    else
        *newNode < *parent ? parent->setLeft(newNode) : parent->setRight(newNode);

    insertFixup(newNode);
}

template <typename T>
void RedBlackTree<T>::remove(const T& key)
{
    auto delNode = this->search(this->root_, key);
    if (delNode == this->nil_)
        return;

    bool originalColorWasBlack = delNode->isBlack();
    auto successor = this->nil_;

    if (delNode->getLeft() == this->nil_)
    {
        successor = delNode->getRight();
        this->transplant(delNode, delNode->getRight());
    }
    else if (delNode->getRight() == this->nil_)
    {
        successor = delNode->getLeft();
        this->transplant(delNode, delNode->getLeft());
    }
    else
    {
        // Another option would be to find maximum of left subtree and rotate accordingly.
        auto midNode = this->findMin(delNode->getRight());
        originalColorWasBlack = midNode->isBlack();
        successor = midNode->getRight();

        if (midNode->getParent() == delNode)
            successor->setParent(midNode);
        else
        {
            this->transplant(midNode, midNode->getRight());
            midNode->setRight(delNode->getRight());
            midNode->getRight()->setParent(midNode);
        }

        this->transplant(delNode, midNode);
        midNode->setLeft(delNode->getLeft());
        midNode->getLeft()->setParent(midNode);
        midNode->recolor(delNode);
    }

    delete delNode;

    if (originalColorWasBlack)
        removeFixup(successor);
}

//
//     |                 |
//     X                 Y
//    / \     LR(X)     / \
//   a   Y   ------>   X   c
//      / \           / \
//     b   c         a   b
//
template <typename T>
void RedBlackTree<T>::leftRotate(Node* node)
{
    auto pivot = node->getRight();
    node->setRight(pivot->getLeft());

    if (pivot->getLeft() != this->nil_)
        pivot->getLeft()->setParent(node);

    pivot->setParent(node->getParent());

    if (node->getParent() == this->nil_)
        this->root_ = pivot;
    else if (node == node->getParent()->getLeft())
        node->getParent()->setLeft(pivot);
    else
        node->getParent()->setRight(pivot);

    pivot->setLeft(node);
    node->setParent(pivot);
}

//
//       |                 |
//       Y                 X
//      / \     RR(Y)     / \
//     X   c   ------>   a   Y
//    / \                   / \
//   a   b                 b   c
//
template <typename T>
void RedBlackTree<T>::rightRotate(Node* node)
{
    auto pivot = node->getLeft();
    node->setLeft(pivot->getRight());

    if (pivot->getRight() != this->nil_)
        pivot->getRight()->setParent(node);

    pivot->setParent(node->getParent());

    if (node->getParent() == this->nil_)
        this->root_ = pivot;
    else if (node == node->getParent()->getRight())
        node->getParent()->setRight(pivot);
    else
        node->getParent()->setLeft(pivot);

    pivot->setRight(node);
    node->setParent(pivot);
}

// Case 1: Uncle is red
// Case 2: Uncle is black and node is right child
// Case 3: Uncle is black and node is left child
template <typename T>
void RedBlackTree<T>::insertFixup(Node* node)
{
    while (node->getParent()->isRed())
    {
        if (node->getParent() == node->getParent()->getParent()->getLeft())
        {
            auto uncle = node->getParent()->getParent()->getRight();

            if (uncle->isRed()) // case 1
            {
                node->getParent()->setColor(Color::Black);
                uncle->setColor(Color::Black);
                node->getParent()->getParent()->setColor(Color::Red);
                node = node->getParent()->getParent();
            }
            else
            {
                if (node == node->getParent()->getRight()) // case 2
                {
                    node = node->getParent();
                    leftRotate(node);
                }

                node->getParent()->setColor(Color::Black); // case 3
                node->getParent()->getParent()->setColor(Color::Red);
                rightRotate(node->getParent()->getParent());
            }
        }
        else if (node->getParent() == node->getParent()->getParent()->getRight())
        {
            auto uncle = node->getParent()->getParent()->getLeft();

            if (uncle->isRed()) // case 1
            {
                node->getParent()->setColor(Color::Black);
                uncle->setColor(Color::Black);
                node->getParent()->getParent()->setColor(Color::Red);
                node = node->getParent()->getParent();
            }
            else
            {
                if (node == node->getParent()->getLeft()) // case 2
                {
                    node = node->getParent();
                    rightRotate(node);
                }

                node->getParent()->setColor(Color::Black); // case 3
                node->getParent()->getParent()->setColor(Color::Red);
                leftRotate(node->getParent()->getParent());
            }
        }
    }

    this->root_->setColor(Color::Black);
}

// Case 1: Sibling is red
// Case 2: Sibling is black with both children black
// Case 3: Sibling is black with red left child and black right child
// Case 4: Sibling is black with red right child
template <typename T>
void RedBlackTree<T>::removeFixup(Node* node)
{
    while (node != this->root_ && node->isBlack())
    {
        if (node == node->getParent()->getLeft())
        {
            auto sibling = node->getParent()->getRight();

            if (sibling->isRed()) // case 1
            {
                sibling->setColor(Color::Black);
                node->getParent()->setColor(Color::Red);
                leftRotate(node->getParent());
                sibling = node->getParent()->getRight();
            }

            if (sibling->getLeft()->isBlack() && sibling->getRight()->isBlack()) // case 2
            {
                sibling->setColor(Color::Red);
                node = node->getParent();
            }
            else
            {
                if (sibling->getRight()->isBlack()) // case 3
                {
                    sibling->getLeft()->setColor(Color::Black);
                    sibling->setColor(Color::Red);
                    rightRotate(sibling);
                    sibling = node->getParent()->getRight();
                }

                sibling->recolor(node->getParent()); // case 4
                node->getParent()->setColor(Color::Black);
                sibling->getRight()->setColor(Color::Black);
                leftRotate(node->getParent());
                node = this->root_;
            }
        }
        else if (node == node->getParent()->getRight())
        {
            auto sibling = node->getParent()->getLeft();

            if (sibling->isRed()) // case 1
            {
                sibling->setColor(Color::Black);
                node->getParent()->setColor(Color::Red);
                rightRotate(node->getParent());
                sibling = node->getParent()->getLeft();
            }

            if (sibling->getLeft()->isBlack() && sibling->getRight()->isBlack()) // case 2
            {
                sibling->setColor(Color::Red);
                node = node->getParent();
            }
            else
            {
                if (sibling->getLeft()->isBlack()) // case 3
                {
                    sibling->getRight()->setColor(Color::Black);
                    sibling->setColor(Color::Red);
                    leftRotate(sibling);
                    sibling = node->getParent()->getLeft();
                }

                sibling->recolor(node->getParent()); // case 4
                node->getParent()->setColor(Color::Black);
                sibling->getLeft()->setColor(Color::Black);
                rightRotate(node->getParent());
                node = this->root_;
            }
        }
    }

    node->setColor(Color::Black);
}
