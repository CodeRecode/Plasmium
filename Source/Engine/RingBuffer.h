#pragma once
#include "Types.h"
#include <assert.h>
#include <new>
#include <string.h>

template <typename T>
class RingBufferIterator {
private:
    T* data;
    uint32 index;
    uint32 capacity;
public:
    RingBufferIterator(T* data, uint32 index, uint32 capacity) :
        data(data),
        index(index),
        capacity(capacity)
    {}

    T& operator*() { return *(data + index); }
    T* operator->() { return data + index; }

    RingBufferIterator& operator++() { index = (index + 1) % capacity; return *this; }
    RingBufferIterator& operator--() { index == 0 ? capacity - 1 : index - 1; return *this; }
    friend bool operator==(const RingBufferIterator& a, const RingBufferIterator& b)
    {
        return a.data == b.data && a.index == b.index;
    }
    friend bool operator!=(const RingBufferIterator & a, const RingBufferIterator & b)
    {
        return !(a == b);
    }
};

template <typename T>
class RingBuffer {
private:
    T* m_data = nullptr;
    uint32 m_size = 0;
    uint32 m_capacity = 0;
    uint32 m_begin = 0;
    uint32 m_end = 0;

    bool IndexInBounds(uint32 index);
    void Copy(const RingBuffer<T>& copy);
    void AdjustSize(uint32 newSize);
    void AdjustCapacity(uint32 newCapacity);

public:
    RingBuffer() {}
    RingBuffer(uint32 capacity) { AdjustCapacity(capacity); }
    RingBuffer(const RingBuffer<T>& copy);
    ~RingBuffer();

    RingBuffer<T>& operator=(const RingBuffer<T>& rhs);
    T& operator[](uint32 index);
    const T& operator[](uint32 index) const;

    RingBufferIterator<T> begin() const
    {
        return RingBufferIterator<T>(m_data, m_begin, m_capacity);
    }
    RingBufferIterator<T> end() const
    { 
        return RingBufferIterator<T>(m_data, m_end, m_capacity);
    }

    uint32 Size() const { return m_size; }
    bool Empty() const { return m_size == 0; }
    uint32 Capacity() const { return m_capacity; }

    T& Front();
    T& Back();
    void Clear();
    void PushBack(const T& object);
    void PushBack(T&& object);
    void PushFront(const T& object);
    void PushFront(T&& object);
    T PopBack();
    T PopFront();
};


template<typename T>
bool RingBuffer<T>::IndexInBounds(uint32 index) {
    if (m_size == 0) {
        return false;
    }
    if (m_end > m_begin) {
        return index >= m_begin && index < m_end;
    }

    return (index >= m_begin && index < m_capacity)
        || (index >= 0 && index < m_end);
}

template<typename T>
void RingBuffer<T>::Copy(const RingBuffer<T>& copy)
{
    m_capacity = copy.m_capacity;
    m_data = (T*)operator new(m_capacity * sizeof(T));
    m_size = copy.m_size;
    m_begin = copy.m_begin;
    m_end = copy.m_end;

    memcpy(m_data, copy.m_data, copy.m_size * sizeof(T));
}

template<typename T>
void RingBuffer<T>::AdjustSize(uint32 newSize)
{
    // Leave one empty so iterators work correctly
    if (newSize + 1 > m_capacity) {
        AdjustCapacity(m_capacity * 2 + 2);
    }

    m_size = newSize;
}

template<typename T>
void RingBuffer<T>::AdjustCapacity(uint32 newCapacity)
{
    T* newData = (T*)operator new(newCapacity * sizeof(T));

    if (m_data) {
        if (m_begin < m_end) {
            memcpy(newData, m_data + m_begin, m_size * sizeof(T));
        }
        else {
            uint32 sizeChunk2 = m_capacity - m_begin;
            memcpy(newData, m_data + m_begin, sizeChunk2 * sizeof(T));
            memcpy(newData + sizeChunk2, m_data, (m_size - sizeChunk2) * sizeof(T));
        }
        operator delete(m_data);
    }

    m_capacity = newCapacity;
    m_data = newData;
    m_begin = 0;
    m_end = m_size;
}

template<typename T>
RingBuffer<T>::RingBuffer(const RingBuffer<T>& copy)
{
    Copy(copy);
}

template<typename T>
RingBuffer<T>::~RingBuffer()
{
    Clear();
}

template<typename T>
RingBuffer<T>& RingBuffer<T>::operator=(const RingBuffer<T>& rhs)
{
    operator delete(m_data);
    Copy(rhs);

    return *this;
}

template<typename T>
T& RingBuffer<T>::operator[](uint32 index)
{
    assert(IndexInBounds(index));
    return m_data[index];
}

template<typename T>
const T& RingBuffer<T>::operator[](uint32 index) const
{
    assert(IndexInBounds(index));
    return m_data[index];
}

template<typename T>
T& RingBuffer<T>::Front()
{
    return m_data[m_begin];
}

template<typename T>
T& RingBuffer<T>::Back()
{
    uint32 back = m_end == 0 ? m_capacity - 1 : m_end - 1;
    return m_data[back];
}

template<typename T>
void RingBuffer<T>::Clear()
{
    for (uint32 index = 0; index < m_size; ++index) {
        (m_data + index)->~T();
    }

    operator delete(m_data);
    m_data = nullptr;
    m_size = 0;
    m_capacity = 0;
    m_begin = 0;
    m_end = 0;
}

template<typename T>
void RingBuffer<T>::PushBack(const T& object)
{
    AdjustSize(m_size + 1);
    new(m_data + m_end) T(object);
    m_end = (m_end + 1) % m_capacity;
}

template<typename T>
void RingBuffer<T>::PushBack(T&& object)
{
    AdjustSize(m_size + 1);
    new(m_data + m_end) T(object);
    m_end = (m_end + 1) % m_capacity;
}

template<typename T>
void RingBuffer<T>::PushFront(const T& object)
{
    AdjustSize(m_size + 1);
    m_begin = m_begin == 0 ? m_capacity - 1 : m_begin - 1;
    new(m_data + m_begin) T(object);
}

template<typename T>
void RingBuffer<T>::PushFront(T&& object)
{
    AdjustSize(m_size + 1);
    m_begin = m_begin == 0 ? m_capacity - 1 : m_begin - 1;
    new(m_data + m_begin) T(object);
}

template<typename T>
T RingBuffer<T>::PopBack()
{
    m_end = m_end == 0 ? m_capacity - 1 : m_end - 1;
    T value(m_data[m_end]);
    (m_data + m_end)->~T();
    AdjustSize(m_size - 1);

    return value;
}

template<typename T>
T RingBuffer<T>::PopFront()
{
    T value(m_data[m_begin]);
    (m_data + m_begin)->~T();
    AdjustSize(m_size - 1);
    m_begin = (m_begin + 1) % m_capacity;

    return value;
}