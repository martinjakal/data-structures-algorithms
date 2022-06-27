#pragma once

#include "Node.hpp"
#include "NodeProperties.hpp"

template <typename T>
class BinaryNode : public Node<T>, public HasDoubleChild<BinaryNode<T>>
{
public:
    using Base = Node<T>;

    BinaryNode(const T& key) : Base(key) {}
};
