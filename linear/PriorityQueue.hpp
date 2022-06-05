#pragma once

#include <sstream>
#include <string>
#include <vector>

template <typename T>
class PriorityQueue
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
    std::vector<T> data_;

    void maxHeapify(std::size_t i = 0);
};

template <typename T>
bool PriorityQueue<T>::empty() const
{
    return data_.empty();
}

template <typename T>
auto PriorityQueue<T>::size() const -> std::size_t
{
    return data_.size();
}

template <typename T>
bool PriorityQueue<T>::contains(const T& value) const
{
    return std::find(data_.begin(), data_.end(), value) != data_.end();
}

template <typename T>
auto PriorityQueue<T>::front() -> T&
{
    if (empty())
        throw std::runtime_error("Priority queue underflow");

    return data_.front();
}

template <typename T>
auto PriorityQueue<T>::front() const -> const T&
{
    if (empty())
        throw std::runtime_error("Priority queue underflow");

    return data_.front();
}

template <typename T>
auto PriorityQueue<T>::back() -> T&
{
    if (empty())
        throw std::runtime_error("Priority queue underflow");

    return data_.back();
}

template <typename T>
auto PriorityQueue<T>::back() const -> const T&
{
    if (empty())
        throw std::runtime_error("Priority queue underflow");

    return data_.back();
}

template <typename T>
void PriorityQueue<T>::push(const T& value)
{
    data_.push_back(value);
    std::size_t i = size() - 1;
    auto parent = [](std::size_t i) { return i / 2; };

    while (i > 0 && data_[parent(i)] < data_[i])
    {
        std::swap(data_[parent(i)], data_[i]);
        i = parent(i);
    }
}

template <typename T>
void PriorityQueue<T>::pop()
{
    if (empty())
        throw std::runtime_error("Priority queue underflow");

    std::swap(data_.front(), data_.back());
    data_.pop_back();
    maxHeapify();
}

template <typename T>
void PriorityQueue<T>::maxHeapify(std::size_t i)
{
    std::size_t maxIdx = i;
    std::size_t leftIdx = 2 * i + 1;
    std::size_t rightIdx = 2 * i + 2;

    if (leftIdx < size() && data_[leftIdx] > data_[maxIdx])
        maxIdx = leftIdx;

    if (rightIdx < size() && data_[rightIdx] > data_[maxIdx])
        maxIdx = rightIdx;

    if (maxIdx != i)
    {
        std::swap(data_[i], data_[maxIdx]);
        maxHeapify(maxIdx);
    }
}

template <typename T>
auto PriorityQueue<T>::print() const -> std::string
{
    std::stringstream out;

    for (const auto& item : data_)
        out << item << (item != back() ? " " : "");
    out << "\n";

    return out.str();
}
