#pragma once

#include <string>

#include "ColorType.hpp"
#include "Node.hpp"
#include "NodeProperties.hpp"

template <typename T>
class RedBlackNode : public Node<T>, public HasParent<RedBlackNode<T>>, public HasDoubleChild<RedBlackNode<T>>
{
public:
    using Base = Node<T>;
    using Node = RedBlackNode<T>;

    RedBlackNode(const T& key) : Base(key), color_(Color::Red) {}
    RedBlackNode() = default;

    bool operator==(const Node& other) const;
    bool operator!=(const Node& other) const;

    void setColor(Color color) { color_ = color; }
    void recolor(Node* node) { color_ = node->color_; }

    bool isRed() const { return color_ == Color::Red; }
    bool isBlack() const { return color_ == Color::Black; }

    auto print() const -> std::string;

private:
    Color color_ = Color::Black;
};

template <typename T>
bool RedBlackNode<T>::operator==(const Node& other) const
{
    return Base::operator==(other) && color_ == other.color_;
}

template <typename T>
bool RedBlackNode<T>::operator!=(const Node& other) const
{
    return !(*this == other);
}

template <typename T>
auto RedBlackNode<T>::print() const -> std::string
{
    return Base::print() + (isBlack() ? "-B" : "-R");
}
