#pragma once

template <typename T>
class Iterator
{
public:
    using ValueType = typename T::ValueType;
    using Pointer = ValueType*;
    using Reference = ValueType&;

    Iterator(Pointer ptr);

    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;

    auto operator++() -> Iterator&;
    auto operator++(int) -> Iterator;
    auto operator--() -> Iterator&;
    auto operator--(int) -> Iterator;

    auto operator[](std::size_t idx) -> Reference;
    auto operator->() -> Pointer;
    auto operator*() -> Reference;

private:
    Pointer ptr_;
};

template <typename T>
Iterator<T>::Iterator(Pointer ptr) : ptr_(ptr)
{}

template <typename T>
bool Iterator<T>::operator==(const Iterator& other) const
{
    return ptr_ == other.ptr_;
}

template <typename T>
bool Iterator<T>::operator!=(const Iterator& other) const
{
    return !(*this == other);
}

template <typename T>
auto Iterator<T>::operator++() -> Iterator&
{
    ++ptr_;
    return *this;
}

template <typename T>
auto Iterator<T>::operator++(int) -> Iterator
{
    auto it = *this;
    ++*this;
    return it;
}

template <typename T>
auto Iterator<T>::operator--() -> Iterator&
{
    --ptr_;
    return *this;
}

template <typename T>
auto Iterator<T>::operator--(int) -> Iterator
{
    auto it = *this;
    --*this;
    return it;
}

template <typename T>
auto Iterator<T>::operator[](std::size_t idx) -> Reference
{
    return *(ptr_ + idx);
}

template <typename T>
auto Iterator<T>::operator->() -> Pointer
{
    return ptr_;
}

template <typename T>
auto Iterator<T>::operator*() -> Reference
{
    return *ptr_;
}
