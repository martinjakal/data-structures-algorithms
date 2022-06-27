#pragma once

#include "ColorType.hpp"
#include "Node.hpp"
#include "NodeProperties.hpp"

template <typename T>
class RedBlackNode : public Node<T>, public HasParent<RedBlackNode<T>>, public HasDoubleChild<RedBlackNode<T>>
{
public:
    using Base = Node<T>;

    RedBlackNode(const T& key) : Base(key), color_(Color::Red) {}
    RedBlackNode() = default;

    void setColor(Color color) { color_ = color; }
    void recolor(RedBlackNode<T>* node) { color_ = node->color_; }

    bool isRed() const { return color_ == Color::Red; }
    bool isBlack() const { return color_ == Color::Black; }

private:
    Color color_ = Color::Black;
};
