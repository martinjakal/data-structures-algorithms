#pragma once

#include <sstream>
#include <string>

#include <linear/Iterator.hpp>

template <typename T>
class DynamicArray
{
public:
    using ValueType = T;
    using Iterator = Iterator<DynamicArray<T>>;

    DynamicArray();
    DynamicArray(const DynamicArray<T>& other);
    DynamicArray(DynamicArray<T>&& other) noexcept;
    DynamicArray<T>& operator=(const DynamicArray<T>& other);
    DynamicArray<T>& operator=(DynamicArray<T>&& other) noexcept;
    ~DynamicArray();

    friend void swap(DynamicArray<T>& lhs, DynamicArray<T>& rhs) noexcept;
    bool operator==(const DynamicArray<T>& other) const;
    bool operator!=(const DynamicArray<T>& other) const;
    auto operator[](std::size_t idx) -> T&;
    auto operator[](std::size_t idx) const -> const T&;

    auto begin() -> Iterator;
    auto end() -> Iterator;
    bool empty() const;
    bool full() const;
    auto size() const -> std::size_t { return size_; }
    auto capacity() const -> std::size_t { return capacity_; }
    void resize();
    void shrink();
    void clear();
    auto print(bool debug = false) const -> std::string;

    bool contains(const T& key) const;
    auto front() -> T&;
    auto front() const -> const T&;
    auto back() -> T&;
    auto back() const -> const T&;

    void push(const T& value);
    void push(T&& value);
    void pushAtPos(std::size_t idx, const T& value);
    void pushAtPos(std::size_t idx, T&& value);
    template <typename... Args>
    auto emplace(Args&&... args) -> T&;
    template <typename... Args>
    auto emplaceAtPos(std::size_t idx, Args&&... args) -> T&;
    void pop();
    void popAtPos(std::size_t idx);

private:
    T* data_ = nullptr;

    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
};

template <typename T>
DynamicArray<T>::DynamicArray()
{
    resize();
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) :
    size_(other.size_),
    capacity_(other.capacity_)
{
    T* newData = (T*)::operator new(capacity_ * sizeof(T));

    for (std::size_t i = 0; i < size_; ++i)
        newData[i] = other.data_[i];

    data_ = newData;
}

template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray<T>&& other) noexcept
{
    swap(*this, other);
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other)
{
    DynamicArray<T> copy(other);
    swap(*this, copy);
    return *this;
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T>&& other) noexcept
{
    swap(*this, other);
    return *this;
}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
    clear();
    ::operator delete(data_, capacity_ * sizeof(T));
}

template <typename T>
void swap(DynamicArray<T>& lhs, DynamicArray<T>& rhs) noexcept
{
    using std::swap;
    swap(lhs.data_, rhs.data_);
    swap(lhs.size_, rhs.size_);
    swap(lhs.capacity_, rhs.capacity_);
}

template <typename T>
bool DynamicArray<T>::operator==(const DynamicArray& other) const
{
    if (size_ != other.size_ || capacity_ != other.capacity_)
        return false;

    for (std::size_t i = 0; i < capacity_; ++i)
    {
        if (data_[i] != other.data_[i])
            return false;
    }
    return true;
}

template <typename T>
bool DynamicArray<T>::operator!=(const DynamicArray& other) const
{
    return !(*this == other); 
}

template <typename T>
auto DynamicArray<T>::operator[](std::size_t idx) -> T&
{
    if (idx >= size_)
        throw std::runtime_error("Index out of range");

    return data_[idx];
}

template <typename T>
auto DynamicArray<T>::operator[](std::size_t idx) const -> const T&
{
    if (idx >= size_)
        throw std::runtime_error("Index out of range");

    return data_[idx];
}

template <typename T>
auto DynamicArray<T>::begin() -> Iterator
{
    return Iterator(data_);
}

template <typename T>
auto DynamicArray<T>::end() -> Iterator
{
    return Iterator(data_ + size_);
}

template <typename T>
bool DynamicArray<T>::empty() const
{
    return size_ == 0;
}

template <typename T>
bool DynamicArray<T>::full() const
{
    return size_ == capacity_;
}

template <typename T>
void DynamicArray<T>::resize()
{
    std::size_t newCapacity = capacity_ != 0 ? capacity_ + capacity_ / 2 : 2; // start with 2, grow by 50%
    T* newData = (T*)::operator new(newCapacity * sizeof(T));

    for (std::size_t i = 0; i < size_; ++i)
    {
        new (&newData[i]) T(std::move(data_[i]));
        data_[i].~T();
    }

    ::operator delete(data_, capacity_ * sizeof(T));
    data_ = newData;
    capacity_ = newCapacity;
}

template <typename T>
void DynamicArray<T>::shrink()
{
    std::size_t newCapacity = size_;
    T* newData = (T*)::operator new(newCapacity * sizeof(T));

    for (std::size_t i = 0; i < size_; ++i)
    {
        new (&newData[i]) T(std::move(data_[i]));
        data_[i].~T();
    }

    ::operator delete(data_, capacity_ * sizeof(T));
    data_ = newData;
    capacity_ = newCapacity;
}

template <typename T>
void DynamicArray<T>::clear()
{
    for (std::size_t i = 0; i < size_; ++i)
        data_[i].~T();

    size_ = 0;
}

template <typename T>
auto DynamicArray<T>::print(bool debug) const -> std::string
{
    std::stringstream out;

    if (data_ != nullptr)
    {
        for (std::size_t i = 0; i < size_; ++i)
            out << data_[i] << (i < size_ - 1 ? " " : "");
    }
    out << std::endl;

    if (debug)
        out << "cap " << capacity_ << " size " << size_ << std::endl;

    return out.str();
}

template <typename T>
bool DynamicArray<T>::contains(const T& value) const
{
    for (std::size_t i = 0; i < size_; ++i)
    {
        if (data_[i] == value)
            return true;
    }
    return false;
}

template <typename T>
auto DynamicArray<T>::front() -> T&
{
    if (empty())
        throw std::runtime_error("Dynamic array underflow");

    return data_[0];
}

template <typename T>
auto DynamicArray<T>::front() const -> const T&
{
    if (empty())
        throw std::runtime_error("Dynamic array underflow");

    return data_[0];
}

template <typename T>
auto DynamicArray<T>::back() -> T&
{
    if (empty())
        throw std::runtime_error("Dynamic array underflow");

    return data_[size_ - 1];
}

template <typename T>
auto DynamicArray<T>::back() const -> const T&
{
    if (empty())
        throw std::runtime_error("Dynamic array underflow");

    return data_[size_ - 1];
}

template <typename T>
void DynamicArray<T>::push(const T& value)
{
    if (full())
        resize();

    new (&data_[size_++]) T(value);
}

template <typename T>
void DynamicArray<T>::push(T&& value)
{
    if (full())
        resize();

    new (&data_[size_++]) T(std::move(value));
}

template <typename T>
void DynamicArray<T>::pushAtPos(std::size_t idx, const T& value)
{
    if (idx >= size_)
        throw std::runtime_error("Index out of range");

    if (full())
        resize();

    new (&data_[size_++]) T(value);

    for (std::size_t i = size_ - 1; i > idx; --i)
        std::swap(data_[i], data_[i - 1]);
}

template <typename T>
void DynamicArray<T>::pushAtPos(std::size_t idx, T&& value)
{
    if (idx >= size_)
        throw std::runtime_error("Index out of range");

    if (full())
        resize();

    new (&data_[size_++]) T(std::move(value));

    for (std::size_t i = size_ - 1; i > idx; --i)
        std::swap(data_[i], data_[i - 1]);
}

template <typename T>
template <typename... Args>
auto DynamicArray<T>::emplace(Args&&... args) -> T&
{
    if (full())
        resize();

    new(&data_[size_]) T(std::forward<Args>(args)...);

    return data_[size_++];
}

template <typename T>
template <typename... Args>
auto DynamicArray<T>::emplaceAtPos(std::size_t idx, Args&&... args) -> T&
{
    if (idx >= size_)
        throw std::runtime_error("Index out of range");

    if (full())
        resize();

    new (&data_[size_++]) T(std::forward<Args>(args)...);

    for (std::size_t i = size_ - 1; i > idx; --i)
        std::swap(data_[i], data_[i - 1]);

    return data_[idx];
}

template <typename T>
void DynamicArray<T>::pop()
{
    if (size_ > 0)
    {
        --size_;
        data_[size_].~T();
    }
}

template <typename T>
void DynamicArray<T>::popAtPos(std::size_t idx)
{
    if (idx >= size_)
        throw std::runtime_error("Index out of range");

    if (size_ > 0)
    {
        for (std::size_t i = idx; i < size_ - 1; ++i)
            std::swap(data_[i], data_[i + 1]);

        --size_;
        data_[size_].~T();
    }
}
