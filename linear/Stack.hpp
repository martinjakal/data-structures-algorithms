#pragma once

#include <list>
#include <sstream>
#include <string>

template <typename T>
class Stack
{
public:
    bool empty() const;
    auto size() const -> std::size_t;
    bool contains(const T& value) const;
    auto top() -> T&;
    auto top() const -> const T&;
    void push(const T& value);
    void pop();
    auto print() const -> std::string;

private:
    std::list<T> data_;
};

template <typename T>
bool Stack<T>::empty() const
{
    return data_.empty();
}

template <typename T>
auto Stack<T>::size() const -> std::size_t
{
    return data_.size();
}

template <typename T>
bool Stack<T>::contains(const T& value) const
{
    return std::find(data_.begin(), data_.end(), value) != data_.end();
}

template <typename T>
auto Stack<T>::top() -> T&
{
    if (empty())
        throw std::runtime_error("Stack underflow");

    return data_.back();
}

template <typename T>
auto Stack<T>::top() const -> const T&
{
    if (empty())
        throw std::runtime_error("Stack underflow");

    return data_.back();
}

template <typename T>
void Stack<T>::push(const T& value)
{
    data_.push_back(value);
}

template <typename T>
void Stack<T>::pop()
{
    if (empty())
        throw std::runtime_error("Stack underflow");

    data_.pop_back();
}

template <typename T>
auto Stack<T>::print() const -> std::string
{
    std::stringstream out;

    for (const auto& item : data_)
        out << item << (item != top() ? " " : "");
    out << "\n";

    return out.str();
}
