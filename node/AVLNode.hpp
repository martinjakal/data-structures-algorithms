#pragma once

#include <string>

#include "Node.hpp"
#include "NodeProperties.hpp"

template <typename T>
class AVLNode : public Node<T>, public HasParent<AVLNode<T>>, public HasDoubleChild<AVLNode<T>>
{
public:
    using Base = Node<T>;

    AVLNode(const T& key) : Base(key) {}
    AVLNode() = default;

    bool operator==(const AVLNode<T>& other) const;
    bool operator!=(const AVLNode<T>& other) const;

    int getHeight() const { return height_; }
    void setHeight(int height) { height_ = height; }

    auto print() const -> std::string;

private:
    int height_ = 0;
};

template <typename T>
bool AVLNode<T>::operator==(const AVLNode<T>& other) const
{
    return Base::operator==(other) && height_ == other.height_;
}

template <typename T>
bool AVLNode<T>::operator!=(const AVLNode<T>& other) const
{
    return !(*this == other);
}

template <typename T>
auto AVLNode<T>::print() const -> std::string
{
    return Base::print() + "_" + std::to_string(height_);
}
