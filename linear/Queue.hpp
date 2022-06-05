#pragma once

#include <list>
#include <sstream>
#include <string>

template <typename T>
class Queue
{
public:
    bool empty() const;
    auto size() const -> std::size_t;
    bool contains(const T& value) const;
    auto front() -> T&;
    auto front() const -> const T&;
    auto back() -> T&;
    auto back() const -> const T&;
    void push(const T& value);
    void pop();
    auto print() const -> std::string;

private:
    std::list<T> data_;
};

template <typename T>
bool Queue<T>::empty() const
{
    return data_.empty();
}

template <typename T>
auto Queue<T>::size() const -> std::size_t
{
    return data_.size();
}

template <typename T>
bool Queue<T>::contains(const T& value) const
{
    return std::find(data_.begin(), data_.end(), value) != data_.end();
}

template <typename T>
auto Queue<T>::front() -> T&
{
    if (empty())
        throw std::runtime_error("Queue underflow");

    return data_.front();
}

template <typename T>
auto Queue<T>::front() const -> const T&
{
    if (empty())
        throw std::runtime_error("Queue underflow");

    return data_.front();
}

template <typename T>
auto Queue<T>::back() -> T&
{
    if (empty())
        throw std::runtime_error("Queue underflow");

    return data_.back();
}

template <typename T>
auto Queue<T>::back() const -> const T&
{
    if (empty())
        throw std::runtime_error("Queue underflow");

    return data_.back();
}

template <typename T>
void Queue<T>::push(const T& value)
{
    data_.push_back(value);
}

template <typename T>
void Queue<T>::pop()
{
    if (empty())
        throw std::runtime_error("Queue underflow");

    data_.pop_front();
}

template <typename T>
auto Queue<T>::print() const -> std::string
{
    std::stringstream out;

    for (const auto& item : data_)
        out << item << (item != back() ? " " : "");
    out << "\n";

    return out.str();
}
