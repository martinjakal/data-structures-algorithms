#pragma once

#include "Node.hpp"
#include "NodeProperties.hpp"

template <typename T>
class NaryNode : public Node<T>, public HasParent<NaryNode<T>>, public HasNChildren<NaryNode<T>>
{
public:
    using Base = Node<T>;

    NaryNode(const T& key) : Base(key) {}
};
