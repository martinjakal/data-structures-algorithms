#pragma once

#include "Node.hpp"
#include "NodeProperties.hpp"

template <typename T>
class LinkedNode : public Node<T>, public HasSingleChild<LinkedNode<T>>
{
public:
    using Base = Node<T>;

    LinkedNode(const T& key) : Base(key) {}
    LinkedNode(const T& key, LinkedNode<T>* next) : Base(key) { this->setNext(next); }
};
