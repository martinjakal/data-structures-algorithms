#pragma once

#include <algorithm>
#include <vector>

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

template <typename Node>
class HasNChildren
{
public:
    auto getChildren() const -> Node* { return children_; }
    void setChildren(const std::vector<Node*>& children) { children_ = children; }

    void addChild(Node* node)
    {
        children_.push_back(node);
    }
    void removeChild(Node* node)
    {
        std::remove_if(children_.begin(), children_.end(), [node](Node* child) { return child == node; });
    }

protected:
    std::vector<Node*> children_;

    HasNChildren() = default;
};

template <typename Node>
class HasHeight
{
public:
    int getHeight() const { return height_; }
    void setHeight(int height) { height_ = height; }

protected:
    int height_ = 0;

    HasHeight() = default;
};
