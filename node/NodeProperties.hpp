#pragma once

template <typename Node>
class HasParent
{
public:
    auto getParent() const -> Node* { return parent_; }
    void setParent(Node* parent) { parent_ = parent; }

protected:
    Node* parent_ = nullptr;

    HasParent() = default;
};

template <typename Node>
class HasSingleChild
{
public:
    auto getNext() const -> Node* { return next_; }
    void setNext(Node* node) { next_ = node; }

protected:
    Node* next_ = nullptr;

    HasSingleChild() = default;
};

template <typename Node>
class HasDoubleChild
{
public:
    auto getLeft() const -> Node* { return left_; }
    void setLeft(Node* node) { left_ = node; }

    auto getRight() const -> Node* { return right_; }
    void setRight(Node* node) { right_ = node; }

protected:
    Node* left_ = nullptr;
    Node* right_ = nullptr;

    HasDoubleChild() = default;
};
