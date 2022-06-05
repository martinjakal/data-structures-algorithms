#pragma once

#include <sstream>
#include <string>

#include <node/LinkedNode.hpp>

template <typename T>
class LinkedList
{
public:
    LinkedList() = default;
    LinkedList(const LinkedList<T>& other);
    LinkedList(LinkedList<T>&& other) noexcept;
    LinkedList<T>& operator=(const LinkedList<T>& other);
    LinkedList<T>& operator=(LinkedList<T>&& other) noexcept;
    ~LinkedList();

    bool empty() const;
    auto size() const -> std::size_t;
    bool contains(const T& value) const;
    auto print() const -> std::string;

    void insert(const T& value);
    void insertAtBegin(const T& value);
    void insertAtPos(std::size_t idx, const T& value);
    void insertAtEnd(const T& value);

    void remove(const T& value);
    void removeAtBegin();
    void removeAtPos(std::size_t idx);
    void removeAtEnd();    

private:
    LinkedNode<T>* head_ = nullptr;
};

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other)
{
    LinkedNode<T>* node = other.head_;

    while (node != nullptr)
    {
        insert(node->getKey());
        node = node->getNext();
    }
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList<T>&& other) noexcept
{
    std::swap(head_, other.head_);
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other)
{
    LinkedList<T> copy(other);
    std::swap(head_, copy.head_);
    return *this;
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T>&& other) noexcept
{
    std::swap(head_, other.head_);
    return *this;
}

template <typename T>
LinkedList<T>::~LinkedList()
{
    while (head_ != nullptr)
    {
        LinkedNode<T>* node = head_;
        head_ = head_->getNext();
        delete node;
    }
}

template <typename T>
bool LinkedList<T>::empty() const
{
    return head_ == nullptr;
}

template <typename T>
auto LinkedList<T>::size() const -> std::size_t
{
    std::size_t nodeCnt = 0;

    for (auto node = head_; node != nullptr; node = node->getNext())
        ++nodeCnt;

    return nodeCnt;
}

template <typename T>
bool LinkedList<T>::contains(const T& value) const
{
    LinkedNode<T>* node = head_;

    while (node != nullptr && node->getKey() != value)
        node = node->getNext();

    return node != nullptr;
}

template <typename T>
void LinkedList<T>::insert(const T& value)
{
    insertAtEnd(value);
}

template <typename T>
void LinkedList<T>::insertAtBegin(const T& value)
{
    head_ = new LinkedNode<T>(value, head_);
}

template <typename T>
void LinkedList<T>::insertAtPos(std::size_t idx, const T& value)
{
    if (idx == 0)
    {
        insertAtBegin(value);
        return;
    }

    if (idx >= size())
        throw std::runtime_error("Index out of range");

    LinkedNode<T>* node = head_;

    for (std::size_t i = 0; i < idx - 1; ++i) // traverse to parent
        node = node->getNext();

    node->setNext(new LinkedNode<T>(value, node->getNext()));
}

template <typename T>
void LinkedList<T>::insertAtEnd(const T& value)
{
    if (empty())
    {
        insertAtBegin(value);
        return;
    }

    LinkedNode<T>* node = head_;

    while (node->getNext() != nullptr)
        node = node->getNext();

    node->setNext(new LinkedNode<T>(value));
}

template <typename T>
void LinkedList<T>::remove(const T& value)
{
    if (empty())
        throw std::runtime_error("Linked list underflow");

    if (head_->getKey() == value)
    {
        removeAtBegin();
        return;
    }

    LinkedNode<T>* parent = nullptr;
    LinkedNode<T>* node = head_;

    while (node->getNext() != nullptr)
    {
        parent = node;
        node = node->getNext();

        if (node->getKey() == value)
        {
            parent->setNext(node->getNext());
            delete node;
            return;
        }
    }
}

template <typename T>
void LinkedList<T>::removeAtBegin()
{
    if (empty())
        throw std::runtime_error("Linked list underflow");

    LinkedNode<T>* node = head_;
    head_ = head_->getNext();
    delete node;
}

template <typename T>
void LinkedList<T>::removeAtPos(std::size_t idx)
{
    if (empty())
        throw std::runtime_error("Linked list underflow");

    if (idx >= size())
        throw std::runtime_error("Index out of range");

    if (idx == 0)
    {
        removeAtBegin();
        return;
    }

    LinkedNode<T>* parent = nullptr;
    LinkedNode<T>* node = head_;

    for (std::size_t i = 0; i < idx; ++i)
    {
        parent = node;
        node = node->getNext();
    }

    parent->setNext(node->getNext());
    delete node;        
}

template <typename T>
void LinkedList<T>::removeAtEnd()
{
    if (empty())
        throw std::runtime_error("Linked list underflow");

    if (head_->getNext() == nullptr)
    {
        removeAtBegin();
        return;
    }

    LinkedNode<T>* parent = nullptr;
    LinkedNode<T>* node = head_;

    while (node->getNext() != nullptr)
    {
        parent = node;
        node = node->getNext();
    }

    parent->setNext(nullptr);
    delete node;
}

template <typename T>
auto LinkedList<T>::print() const -> std::string
{
    std::stringstream out;

    for (auto node = head_; node != nullptr; node = node->getNext())
        out << node->getKey() << (node->getNext() != nullptr ? " " : "");
    out << "\n";

    return out.str();
}
