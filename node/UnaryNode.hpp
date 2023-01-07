#pragma once

#include "Node.hpp"
#include "NodeProperties.hpp"

template <typename T>
class UnaryNode : public Node<T>, public HasSingleChild<UnaryNode<T>>
{
public:
    using Base = Node<T>;

    UnaryNode(const T& key) : Base(key) {}
    UnaryNode(const T& key, UnaryNode<T>* next) : Base(key) { this->setNext(next); }
};
